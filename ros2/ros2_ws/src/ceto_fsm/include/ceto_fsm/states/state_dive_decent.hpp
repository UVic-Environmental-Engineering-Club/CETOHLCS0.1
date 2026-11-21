#pragma once

#include "ceto_fsm/ceto_state.hpp"
#include "ceto_interfaces/msg/control_setpoints.hpp"

#include <chrono>

class StateDiveDecent : public CETOState {
public:
    ceto_interfaces::msg::ControlSetpoints on_enter(CETOContext& context) override;

    StateEnum check_transitions(CETOContext& context) override;

    void on_exit(CETOContext& context) override;
    
    StateEnum get_state_id() const override;

private:
    int _dive_decent_time = 0; // Time to remain in DIVE_DECENT state, in milliseconds
    std::chrono::time_point<std::chrono::system_clock> _start_time; // Time when the state was entered
};
