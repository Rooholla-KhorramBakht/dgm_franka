/**
 * @file simple_dgm.hpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @license License BSD-3-Clause
 * @copyright Copyright (c) 2019, New York University and Max Planck
 * Gesellschaft.
 * @date 2019-05-22
 */
#include "dgm_fr3_kin.hh"
namespace dynamic_graph_manager
{

    void DGMFrankaKin::franka_update_sensors(const franka::RobotState& states, double stamp)
    {
        robot_state_ = states;
        franka_stamp_ = stamp;
    }

    void DGMFrankaKin::franka_get_cmd(dynamicgraph::Vector &cmd, double &stamp)
    {
        // return robot_cmd_;
        cmd = desired_velocities_; 
        stamp = ctrl_stamp_;
    }

    void DGMFrankaKin::initialize_hardware_communication_process()
    {

    }

    void DGMFrankaKin::get_sensors_to_map(VectorDGMap& map)
    {
        map["timestamp"][0] =  franka_stamp_;
        map["joint_positions"] =  Eigen::Map<Eigen::VectorXd>(robot_state_.q.data(), 7);
        map["joint_velocities"] = Eigen::Map<Eigen::VectorXd>(robot_state_.dq.data(), 7);
        map["joint_torques"] = Eigen::Map<Eigen::VectorXd>(robot_state_.tau_J_d.data(), 7);
    }

    void DGMFrankaKin::set_motor_controls_from_map(const VectorDGMap& map)
    {
        desired_velocities_ = map.at("ctrl_joint_velocities");
        ctrl_stamp_ = map.at("ctrl_stamp")[0];
    }

    bool DGMFrankaKin::is_in_safety_mode()
    {
        // here define a check for robot specific stuff.
        bool check = false;
        return check || DynamicGraphManager::is_in_safety_mode();
    }

    void DGMFrankaKin::compute_safety_controls()
    {
        // fill all controls with zero... Check your robot to imagine what would
        // be safer in this case.
        for (VectorDGMap::iterator ctrl = motor_controls_map_.begin();
             ctrl != motor_controls_map_.end();
             ++ctrl)
        {
            ctrl->second.fill(0.0);
        }
    }

}  // dg_franka_dyn