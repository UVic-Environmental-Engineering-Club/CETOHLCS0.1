#pragma once

#include "ceto_fsm/ceto_state.hpp"

#include <chrono>

class StateStandby : public CETOState {
public:
    void on_enter(CETOContext& context) override;

    ceto_interfaces::msg::ControlSetpoints execute(CETOContext& context) override;

    StateEnum check_transitions(CETOContext& context) override;

    void on_exit(CETOContext& context) override;
    
    StateEnum get_state_id() const override;

private:
    int _standby_time = 0; // Time to remain in STANDBY state, in milliseconds
    std::chrono::time_point<std::chrono::system_clock> _start_time; // Time when the state was entered
};
