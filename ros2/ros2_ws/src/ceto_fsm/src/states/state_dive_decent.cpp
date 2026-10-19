#include "ceto_fsm/states/state_dive_decent.hpp"

#include <chrono>

ceto_interfaces::msg::ControlSetpoints StateDiveDecent::on_enter(CETOContext& context) {
    this->_dive_decent_time = context.waypoints[context.current_waypoint_index].decent_time;
    this->_start_time = std::chrono::system_clock::now();

    ceto_interfaces::msg::ControlSetpoints setpoints;
    // Populate setpoints as needed    
    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;
    const auto& waypoint = context.waypoints[context.current_waypoint_index];

    if (waypoint.depth_phase == 1)
    {
        setpoints.depth_control_phase = 1;
        setpoints.target_ballast_setting = waypoint.decent_ballast_setting;
    }

    if(waypoint.pitch_phase == 1)
    {
        setpoints.pitch_control_phase = 1;
        setpoints.target_pitch_setting = waypoint.decent_pitch_setting;
    }

    if(waypoint.roll_phase == 1)
    {
        setpoints.roll_control_phase = 1;
        setpoints.target_roll_setting = waypoint.decent_roll_setting;
    }

    return setpoints;
}

StateEnum StateDiveDecent::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the DIVE_DECENT state
    // Return the next state based on conditions

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - _start_time).count();
    
    if (elapsed_time >= _dive_decent_time) {
        return StateEnum::DIVE_ACCENT; // Transition to DIVE_ACCENT after dive decent time
    }

    return StateEnum::DIVE_DECENT;
}

void StateDiveDecent::on_exit(CETOContext& context) {
    // Implementation for exiting the DIVE_DECENT state
}

StateEnum StateDiveDecent::get_state_id() const {
    return StateEnum::DIVE_DECENT;
}
