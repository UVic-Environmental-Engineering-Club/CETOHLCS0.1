#include "ceto_fsm/states/state_on.hpp"
#include "ceto_interfaces/msg/glider_waypoint.hpp"

#define STANDBY_PITCH_SETTING 0.0
#define STANDBY_ROLL_SETTING 0.0
#define STANDBY_BALLAST_SETTING 10.0

ceto_interfaces::msg::ControlSetpoints StateOn::on_enter(CETOContext& context) {

    ceto_interfaces::msg::ControlSetpoints setpoints;

    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_DISABLED;

    return setpoints;

}

StateEnum StateOn::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the ON state
    // Return the next state based on conditions

    if(context.stm32_state == 1) {
        return StateEnum::STANDBY;
    }

    return StateEnum::STANDBY;
    //return StateEnum::ON;
}

void StateOn::on_exit(CETOContext& context) {
    // Implementation for exiting the ON state
}

void StateOn::on_control_command(
    CETOContext&,
    const ceto_interfaces::srv::FSMControl::Request&,
    ceto_interfaces::srv::FSMControl::Response& response) {
    response.success = true;
}

StateEnum StateOn::get_state_id() const {
    return StateEnum::ON;
}