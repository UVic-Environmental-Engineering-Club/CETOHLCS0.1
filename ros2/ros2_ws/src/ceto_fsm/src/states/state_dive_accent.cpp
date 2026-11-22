#include "ceto_fsm/states/state_dive_accent.hpp"

#include <chrono>

ceto_interfaces::msg::ControlSetpoints StateDiveAccent::on_enter(CETOContext& context) {
    this->_dive_ascent_time = context.waypoints[context.current_waypoint_index].ascent_time;
    this->_start_time = std::chrono::system_clock::now();

    ceto_interfaces::msg::ControlSetpoints setpoints;
    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;
    const auto& waypoint = context.waypoints[context.current_waypoint_index];

    if (waypoint.depth_phase == 1)
    {
        setpoints.depth_control_phase = 1;
        setpoints.target_ballast_setting = waypoint.ascent_ballast_setting;
    }

    if (waypoint.pitch_phase == 1)
    {
        setpoints.pitch_control_phase = 1;
        setpoints.target_pitch_setting = waypoint.ascent_pitch_setting;
    }

    if (waypoint.roll_phase == 1)
    {
        setpoints.roll_control_phase = 1;
        setpoints.target_roll_setting = waypoint.ascent_roll_setting;
    }

    return setpoints;
}

StateEnum StateDiveAccent::check_transitions(CETOContext& context) {
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - _start_time).count();

    if (elapsed_time >= _dive_ascent_time) {
        return StateEnum::SURFACE; // Transition to SURFACE after dive ascent time
    }

    return StateEnum::DIVE_ACCENT;
}

void StateDiveAccent::on_exit(CETOContext& context) {
    context.current_waypoint_index++;
}

void StateDiveAccent::on_control_command(
    CETOContext&,
    const ceto_interfaces::srv::FSMControl::Request&,
    ceto_interfaces::srv::FSMControl::Response& response) {
    response.success = true;
}

StateEnum StateDiveAccent::get_state_id() const {
    return StateEnum::DIVE_ACCENT;
}
