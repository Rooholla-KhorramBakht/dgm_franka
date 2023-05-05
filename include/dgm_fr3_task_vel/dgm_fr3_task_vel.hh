/**
 * @file simple_dgm.hpp
 * @author Rooholla Khorrambakht (rk4342@nyu.edu)
 * @license License BSD-3-Clause
 * @copyright Copyright (c) 2019, New York University and Max Planck
 * Gesellschaft.
 * @date 2022-12-09
 */
#include "dynamic_graph_manager/dynamic_graph_manager.hpp"

#include <franka/duration.h>
#include <franka/exception.h>
#include <franka/model.h>
#include <franka/rate_limiting.h>
#include <franka/robot.h>

namespace dynamic_graph_manager
{
/**
 * @brief This class is a dynamic graph manager with a fake hardware
 * interface as the device.
 */
class DGMFrankaKin : public DynamicGraphManager
{
public:
  /**
   * @brief Construct a new FrankaDynamicDGM object
   */
  DGMFrankaKin() : DynamicGraphManager()
  {
    // boolean_set_by_user_cmd_ = false;
  }
  /**
   * @brief Destroy the FrankaDynamicDGM object
   */
  ~DGMFrankaKin()
  {
  }

  void franka_update_sensors(const franka::RobotState& states, double stamp);

  void franka_get_cmd(dynamicgraph::Vector& cmd, double& stamp);

  /**
   * @brief Simple overload doing nothing. We have no hardware here for the
   * unit tests.
   */
  void initialize_hardware_communication_process();
  /**
   * @brief Get the sensors to the map object.
   *
   * @param map of sensors
   */
  void get_sensors_to_map(VectorDGMap& map);
  /**
   * @brief Set the motor controls from map object to no hardware. So nothing
   * to be done here
   *
   * @param map of controls
   */
  void set_motor_controls_from_map(const VectorDGMap& map);

  /**
   * @brief is_in_safety_mode check if the dynamic graph is still alive and
   * sending commands at a descent frequency. Inheriting this method is not
   * mandatory but recommanded.
   * @return true if there is a problem
   */
  bool is_in_safety_mode();

  /**
   * @brief compute_safety_controls computes safety controls very fast in case
   * the dynamic graph is taking to much computation time or has crashed.
   */
  void compute_safety_controls();

private:
  // Control
  dynamicgraph::Vector desired_velocities_;
  double ctrl_stamp_;

  // Sensor and Actuator variables that priodically get updated by libfranka
  franka::RobotState robot_state_;  // Also add the robot model
  double franka_stamp_;
};

}  // namespace dynamic_graph_manager