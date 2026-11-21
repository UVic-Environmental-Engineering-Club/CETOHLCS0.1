#pragma once

#include "ceto_fsm/ceto_state.hpp"
#include "ceto_interfaces/msg/control_setpoints.hpp"

class StateOn : public CETOState {
public:
    ceto_interfaces::msg::ControlSetpoints on_enter(CETOContext& context) override;

    StateEnum check_transitions(CETOContext& context) override;

    void on_exit(CETOContext& context) override;
    
    StateEnum get_state_id() const override;
};