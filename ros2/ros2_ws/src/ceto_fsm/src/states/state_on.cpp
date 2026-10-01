#include "ceto_fsm/states/state_on.hpp"

void StateOn::on_enter(CETOContext& context) {
    // Implementation for entering the ON state
}

ceto_interfaces::msg::ControlSetpoints StateOn::execute(CETOContext& context) {
    // Implementation for executing the ON state
    ceto_interfaces::msg::ControlSetpoints setpoints;
    // Populate setpoints as needed
    return setpoints;
}

StateEnum StateOn::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the ON state
    // Return the next state based on conditions

    if(context.stm32_state == 1) {
        return StateEnum::STANDBY; // Transition to STANDBY if the STM32 state is 1
    }

    return StateEnum::ON; // Placeholder, replace with actual logic
}

void StateOn::on_exit(CETOContext& context) {
    // Implementation for exiting the ON state
}

StateEnum StateOn::get_state_id() const {
    return StateEnum::ON;
}