#include "ceto_fsm/states/state_standby.hpp"

#include <chrono>

#define STANDBY_PITCH_SETTING 0.0
#define STANDBY_ROLL_SETTING 0.0
#define STANDBY_BALLAST_SETTING 10.0

void StateStandby::on_enter(CETOContext& context) {
    this->_standby_time = context.waypoints[context.current_waypoint_index].standby_time;
    this->_start_time = std::chrono::system_clock::now();
}

ceto_interfaces::msg::ControlSetpoints StateStandby::execute(CETOContext& context) {
    // Implementation for executing the STANDBY state
    ceto_interfaces::msg::ControlSetpoints setpoints;
    // Populate setpoints as needed

    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;

    setpoints.target_ballast_setting = STANDBY_BALLAST_SETTING;
    setpoints.target_pitch_setting = STANDBY_PITCH_SETTING;
    setpoints.target_roll_setting = STANDBY_ROLL_SETTING;

    return setpoints;
}

StateEnum StateStandby::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the STANDBY state
    // Return the next state based on conditions

    if (context.current_waypoint_index >= context.waypoints.size()) {
        return StateEnum::STANDBY; // No more waypoints, stay in STANDBY
    }

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - _start_time).count();
    
    if (elapsed_time >= _standby_time) {
        return StateEnum::DIVE_DECENT; // Transition to DIVE_DECENT after standby time
    }

    return StateEnum::STANDBY;
}

void StateStandby::on_exit(CETOContext& context) {
    // Implementation for exiting the STANDBY state
}

StateEnum StateStandby::get_state_id() const {
    return StateEnum::STANDBY;
}
