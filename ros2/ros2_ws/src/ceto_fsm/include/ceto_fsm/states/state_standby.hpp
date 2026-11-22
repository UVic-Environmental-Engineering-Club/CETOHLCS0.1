#pragma once

#include "ceto_fsm/ceto_state.hpp"
#include "ceto_interfaces/msg/control_setpoints.hpp"
#include "ceto_interfaces/srv/load_mission.hpp"

#include "ceto_interfaces/srv/fsm_control.hpp"

#include <chrono>

class StateStandby : public CETOState {
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
    using LoadMissionClient = rclcpp::Client<ceto_interfaces::srv::LoadMission>;

    void responseCallback(LoadMissionClient::SharedFuture future, CETOContext& context);

    std::chrono::time_point<std::chrono::system_clock> _start_time; // Time when the state was entered

    bool _mission_requested = false; // Flag to indicate if the mission has been requested
    bool _mission_loaded = false; // Flag to indicate if the mission has been loaded

};
