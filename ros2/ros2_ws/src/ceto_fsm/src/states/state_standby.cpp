#include "ceto_fsm/states/state_standby.hpp"

#define STANDBY_PITCH_SETTING 0.0
#define STANDBY_ROLL_SETTING 0.0
#define STANDBY_BALLAST_SETTING 10.0

void StateStandby::on_enter(CETOContext& context) {
    // Implementation for entering the STANDBY state
}

ceto_interfaces::msg::ControlSetpoints StateStandby::execute(CETOContext& context) {
    // Implementation for executing the STANDBY state
    ceto_interfaces::msg::ControlSetpoints setpoints;
    // Populate setpoints as needed

    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;

    setpoints.target_ballast_setting = STANDBY_PITCH_SETTING;
    setpoints.target_pitch_setting = STANDBY_ROLL_SETTING;
    setpoints.target_roll_setting = STANDBY_BALLAST_SETTING;

    return setpoints;
}

StateEnum StateStandby::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the STANDBY state
    // Return the next state based on conditions
    return StateEnum::STANDBY; // Placeholder, replace with actual logic
}

void StateStandby::on_exit(CETOContext& context) {
    // Implementation for exiting the STANDBY state
}

StateEnum StateStandby::get_state_id() const {
    return StateEnum::STANDBY;
}
