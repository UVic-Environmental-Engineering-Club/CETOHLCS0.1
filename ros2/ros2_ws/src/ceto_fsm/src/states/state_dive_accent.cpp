#include "ceto_fsm/states/state_dive_accent.hpp"

#include <chrono>

void StateDiveAccent::on_enter(CETOContext& context) {
    this->_dive_accent_time = context.waypoints[context.current_waypoint_index].accent_time;
    this->_start_time = std::chrono::system_clock::now();
}

ceto_interfaces::msg::ControlSetpoints StateDiveAccent::execute(CETOContext& context) {
    ceto_interfaces::msg::ControlSetpoints setpoints;
    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;
    const auto& waypoint = context.waypoints[context.current_waypoint_index];

    if (waypoint.depth_phase == 1)
    {
        setpoints.target_ballast_setting = waypoint.accent_ballast_setting;
    }

    if (waypoint.pitch_phase == 1)
    {
        setpoints.target_pitch_setting = waypoint.accent_pitch_setting;
    }

    if (waypoint.roll_phase == 1)
    {
        setpoints.target_roll_setting = waypoint.accent_roll_setting;
    }

    return setpoints;
}

StateEnum StateDiveAccent::check_transitions(CETOContext& context) {
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - _start_time).count();

    if (elapsed_time >= _dive_accent_time) {
        context.current_waypoint_index++;
        return StateEnum::STANDBY; // Transition to STANDBY after dive accent time
    }

    return StateEnum::DIVE_ACCENT;
}

void StateDiveAccent::on_exit(CETOContext& context) {
    // No state-specific cleanup is needed.
}

StateEnum StateDiveAccent::get_state_id() const {
    return StateEnum::DIVE_ACCENT;
}
