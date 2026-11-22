#pragma once

#include "ceto_fsm/ceto_state.hpp"
#include "ceto_interfaces/msg/control_setpoints.hpp"

#include "ceto_interfaces/srv/fsm_control.hpp"

#include <chrono>

class StateDiveAccent : public CETOState {
public:
    ceto_interfaces::msg::ControlSetpoints on_enter(CETOContext& context) override;

    StateEnum check_transitions(CETOContext& context) override;

    void on_exit(CETOContext& context) override;

    void on_control_command(
        CETOContext& context,
        const ceto_interfaces::srv::FSMControl::Request& request,
        ceto_interfaces::srv::FSMControl::Response& response) override;

    StateEnum get_state_id() const override;

private:
    int _dive_ascent_time = 0; // Time to remain in DIVE_ACCENT state, in milliseconds
    std::chrono::time_point<std::chrono::system_clock> _start_time; // Time when the state was entered
};
