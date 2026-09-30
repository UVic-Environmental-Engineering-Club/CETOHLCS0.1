#pragma once

#include "ceto_fsm/ceto_fsm_state.hpp"

class StateOn : public CETOState {
public:
    void on_enter(CETOContext& context) override;

    glider_interfaces::msg::ControlSetpoints execute(CETOContext& context) override;

    StateEnum check_transitions(CETOContext& context) override;

    void on_exit(CETOContext& context) override;
    
    StateEnum get_state_id() const override;
};