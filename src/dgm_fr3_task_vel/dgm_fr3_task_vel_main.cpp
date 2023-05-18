#include <array>
#include <atomic>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <mutex>
#include <thread>

#include <franka/duration.h>
#include <franka/exception.h>
#include <franka/model.h>
#include <franka/rate_limiting.h>
#include <franka/robot.h>
#include "dgm_fr3_task_vel/dgm_fr3_task_vel.hh"
#include "franka_trajectory_utils.hpp"
#include "lcm/lcm-cpp.hpp"
#include "ipc_trigger_t.hpp"
#include <signal.h>

#include <chrono>
namespace
{
template <class T, size_t N>
std::ostream& operator<<(std::ostream& ostream, const std::array<T, N>& array)
{
  ostream << "[";
  std::copy(array.cbegin(), array.cend() - 1, std::ostream_iterator<T>(ostream, ","));
  std::copy(array.cend() - 1, array.cend(), std::ostream_iterator<T>(ostream));
  ostream << "]";
  return ostream;
}
}  // anonymous namespace

dynamic_graph_manager::DGMFrankaKin dgm;

void signal_callback_handler(int signum)
{
  //  std::cout << "Caught signal " << signum << std::endl;
  std::cout << "\nStopping the DGM and Leaving the application\n" << std::endl;
  dynamic_graph_manager::ros_shutdown();
  exit(signum);
}

/**
 * @example franka_dynamic_dgm.cpp.cpp
 * An executable that connects to a single Franka Emika robot arm and connects it the
 * a shared memory managed by dynamic graph manager. In python, we read the sensor values
 * from and write the actuator commands to this memory. This shared memory acts as an intermediary
 * buffer that allows the Franka real-time system to be updated in time and adapt the rate of python
 * execution with the rate of high level control commands.
 */

int main(int argc, char** argv)
{
  // Check whether the required arguments were passed.
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <robot-hostname>"
              << "<dgm_robot_config.yaml" << std::endl;
    return -1;
  }
  // assign a handler for detecting control-c such that the the dynamic graph can be put to sleep
  signal(SIGINT, signal_callback_handler);

  // Load the DGM parameters and start the graph in a separate thread
  std::string yaml_params_file = std::string(argv[2]);
  std::cout << "Loading parameters from " << yaml_params_file << std::endl;

  YAML::Node param = YAML::LoadFile(yaml_params_file);
  dgm.initialize(param);

  std::thread dgm_thread([] {
    dgm.run();
    std::cout << "DGM is stopped now" << std::endl;
  });
  // wait a second for the dgm to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Initiate the LCM IPC communication channel for triggering the python class
  lcm::LCM lcm;
  if (!lcm.good())
  {
    std::cout << "Can not initialize the LCM connection! " << yaml_params_file << std::endl;
    return -1;
  }

  exlcm::ipc_trigger_t trigger_msg;
  trigger_msg.timestamp = 0;

  // Start communicating with the robot hardware initiate the controller loop
  try
  {
    // Connect to robot.
    franka::Robot robot(argv[1]);

    setDefaultBehavior(robot);

    // First move the robot to a suitable joint configuration
    std::array<double, 7> q_goal = { { 0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4 } };
    MotionGenerator motion_generator(0.5, q_goal);
    system("clear");
    std::cout << "WARNING: This example will move the robot! "
              << "Please make sure to have the user stop button at hand!" << std::endl
              << "Press Enter to continue..." << std::endl;
    std::cin.ignore();

    robot.control(motion_generator);
    std::cout << "Finished moving to initial joint configuration." << std::endl;

    // Set additional parameters always before the control loop, NEVER in the control loop!
    // Set collision behavior.
    robot.setCollisionBehavior(
        { { 20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0 } }, { { 20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0 } },
        { { 20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0 } }, { { 20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0 } },
        { { 20.0, 20.0, 20.0, 25.0, 25.0, 25.0 } }, { { 20.0, 20.0, 20.0, 25.0, 25.0, 25.0 } },
        { { 20.0, 20.0, 20.0, 25.0, 25.0, 25.0 } }, { { 20.0, 20.0, 20.0, 25.0, 25.0, 25.0 } });

    auto stamp_start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point stamp_now = std::chrono::high_resolution_clock::now();
    Eigen::VectorXd cmd(7);
    double control_stamp = 0;

    // Control callback

    robot.control([=, &param, &trigger_msg, &lcm, &cmd, &control_stamp, &stamp_now, &stamp_start]
    (const franka::RobotState& state, franka::Duration) -> franka::CartesianVelocities {
      
      // Get the current CPU time
      stamp_now = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> duration = stamp_now.time_since_epoch();  //- stamp_start;

      // Send the sensor data to the DGM
      dgm.franka_update_sensors(state, duration.count());

      // Send an IPC trigger to serve as a time source for the python controller
      trigger_msg.timestamp =
          std::chrono::duration_cast<std::chrono::microseconds>(stamp_now.time_since_epoch()).count();
      lcm.publish(param["device"]["name"].as<std::string>() + "_trigger", &trigger_msg);
      // Trigger the DGM to read the sensor and update the shared memory with the control command
      dgm.hw_trigger();
      dgm.wait_for_control();
      // Get the control command from the shared memory
      dgm.franka_get_cmd(cmd, control_stamp);

      // //How recent is the computed control command?
      double control_lag = duration.count() - control_stamp;
      // Apply the control command to the robot only if the command is recent enough
      std::array<double, 6> cmd_raw;
      if (abs(control_lag) < 0.1)
      {
        for(int i=0; i<6; i++)
          cmd_raw[i] = cmd(i);
      }
      else
      {
        for(int i=0; i<6; i++)
          // output.O_dP_EE[i] = 0;
          cmd_raw[i] = 0;
      }
      // Limit the commanded velocity such that it is made to stay within limits
      std::array<double, 6> limitted_vel_cmd =  franka::limitRate(
                                                3.0,//franka::kMaxTranslationalVelocity,
                                                9.0,//franka::kMaxTranslationalAcceleration, 
                                                4500.0,//franka::kMaxTranslationalJerk, 
                                                2.5,//franka::kMaxRotationalVelocity, 
                                                17.0,//franka::kMaxRotationalAcceleration, 
                                                8500.0,//franka::kMaxRotationalJerk, 
                                                cmd_raw,         
                                                state.O_dP_EE_c,    
                                                state.O_ddP_EE_c);  
      
      franka::CartesianVelocities output = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
      output.O_dP_EE = limitted_vel_cmd;
      return output;
    });

    // Start the dynamic graph thread
    dynamic_graph_manager::ros_spin();
  }
  catch (const franka::Exception& ex)
  {
    // stop the DGM
    dynamic_graph_manager::ros_shutdown();
    std::cerr << ex.what() << std::endl;
  }

  return 0;
}
