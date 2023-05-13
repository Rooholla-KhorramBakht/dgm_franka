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
#include "dgm_fr3_joint_dyn/dgm_fr3_joint_dyn.hh"
#include "franka_trajectory_utils.hpp"
#include "lcm/lcm-cpp.hpp"
#include "ipc_trigger_t.hpp"
#include <signal.h>
#include <chrono>
#include "lcm_msgs/fr3_states/fr3_joint_state.hpp"
#include "lcm_msgs/fr3_commands/fr3_joint_cmd.hpp"
#include <sys/select.h>


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

dynamic_graph_manager::DGMFrankaDyn dgm;
bool running = true;

void signal_callback_handler(int signum)
{
  //  std::cout << "Caught signal " << signum << std::endl;
  std::cout << "\nStopping the DGM and Leaving the application\n" << std::endl;
  dynamic_graph_manager::ros_shutdown();
  running = false;
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

// LCM communication channel subsystem
class LCMCommandHandler 
{
    public:
        ~LCMCommandHandler() {}
        uint64_t cmd_stamp;
        double cmd_shm[7];
        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const fr3_commands::fr3_joint_cmd* msg)
        {
          memcpy(cmd_shm, msg->cmd, 7);
          cmd_stamp = msg->timestamp;
          std::cout << cmd_stamp << std::endl;
        }
};

void lcmThreadFunc(lcm::LCM *lcm) {
    while(running)
    {
      lcm->handle();
    }
}

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
  fr3_states::fr3_joint_state lcm_joint_state_msg;
  LCMCommandHandler cmdShm;
  lcm.subscribe(param["device"]["name"].as<std::string>() + "_command", &LCMCommandHandler::handleMessage, &cmdShm);
  std::thread lcmThread(lcmThreadFunc, &lcm);


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

    // Define callback for control loop.
    std::function<franka::Torques(const franka::RobotState&, franka::Duration)> control_callback =
        [&param, &trigger_msg, &lcm, &cmd, &control_stamp, &stamp_now,
         &stamp_start, &lcm_joint_state_msg](const franka::RobotState& state, franka::Duration /*period*/) -> franka::Torques {
      // Get the current CPU time
      stamp_now = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> duration = stamp_now.time_since_epoch();  // - stamp_start;
      
      // load the LCM state message to be transmitted
      lcm_joint_state_msg.timestamp=
      std::chrono::duration_cast<std::chrono::microseconds>(stamp_now.time_since_epoch()).count();
      memcpy(lcm_joint_state_msg.q, state.q.data(), 7);
      memcpy(lcm_joint_state_msg.dq, state.dq.data(), 7);
      memcpy(lcm_joint_state_msg.T, state.tau_J.data(), 7);
      lcm.publish(param["device"]["name"].as<std::string>() + "_states", &lcm_joint_state_msg);

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

      std::array<double, 7> tau_d_calculated;

      // Apply the control command to the robot only if the command is recent enough
      if (control_lag < 0.1)
      {
        // std::cout << 1 << std::endl;
        for (size_t i = 0; i < 7; i++)
        {
          tau_d_calculated[i] = cmd(i);
        }
      }
      else
      {
        // std::cout << 0 << std::endl;
        for (size_t i = 0; i < 7; i++)
        {
          tau_d_calculated[i] = 0;
        }
      }

      // Maybe I should remove the following to prevent hampered performance.

      // The following line is only necessary for printing the rate limited torque. As we activated
      // rate limiting for the control loop (activated by default), the torque would anyway be
      // adjusted!
      std::array<double, 7> tau_d_rate_limited =
          franka::limitRate(franka::kMaxTorqueRate, tau_d_calculated, state.tau_J_d);

      // Send torque command.
      return tau_d_rate_limited;
    };

    // Start real-time control loop.
    robot.control(control_callback);
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
