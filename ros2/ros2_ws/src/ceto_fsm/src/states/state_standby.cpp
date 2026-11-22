#include "ceto_fsm/states/state_standby.hpp"

#include <chrono>

#define STANDBY_PITCH_SETTING 0.0
#define STANDBY_ROLL_SETTING 0.0
#define STANDBY_BALLAST_SETTING 10.0

ceto_interfaces::msg::ControlSetpoints StateStandby::on_enter(CETOContext& context) {

    ceto_interfaces::msg::ControlSetpoints setpoints;

    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;

    setpoints.depth_control_phase = 1; 
    setpoints.pitch_control_phase = 1;
    setpoints.roll_control_phase = 1;

    setpoints.target_ballast_setting = STANDBY_BALLAST_SETTING;
    setpoints.target_pitch_setting = STANDBY_PITCH_SETTING;
    setpoints.target_roll_setting = STANDBY_ROLL_SETTING;

    return setpoints;
}

StateEnum StateStandby::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the STANDBY state
    // Return the next state based on conditions

    //Check for start mission command

    

    return StateEnum::STANDBY;
}

void StateStandby::on_exit(CETOContext& context) {
    // Implementation for exiting the STANDBY state
}

StateEnum StateStandby::get_state_id() const {
    return StateEnum::STANDBY;
}
