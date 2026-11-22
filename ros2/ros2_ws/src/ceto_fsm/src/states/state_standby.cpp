#include "ceto_fsm/states/state_standby.hpp"

#define STANDBY_PITCH_SETTING 0.0
#define STANDBY_ROLL_SETTING 0.0
#define STANDBY_BALLAST_SETTING 10.0

ceto_interfaces::msg::ControlSetpoints StateStandby::on_enter(CETOContext& context) {

    ceto_interfaces::msg::ControlSetpoints setpoints;

    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_ACTIVE;

    setpoints.depth_control_phase = 1; 
    setpoints.pitch_control_phase = 1;
    setpoints.roll_control_phase = 1;

    setpoints.target_ballast_setting = STANDBY_BALLAST_SETTING;
    setpoints.target_pitch_setting = STANDBY_PITCH_SETTING;
    setpoints.target_roll_setting = STANDBY_ROLL_SETTING;

    return setpoints;
}

StateEnum StateStandby::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the STANDBY state
    // Return the next state based on conditions

    //Check for start mission command

    if(_mission_loaded) {
        return StateEnum::SURFACE;
    }
    return StateEnum::STANDBY;

}

void StateStandby::responseCallback(LoadMissionClient::SharedFuture future, CETOContext& context) {
    auto response = future.get();
    if (response->success) {
        RCLCPP_INFO(rclcpp::get_logger("StateStandby"), "Mission loaded successfully.");
        _mission_loaded = true;
        context.waypoints = response->waypoints;
        RCLCPP_INFO(rclcpp::get_logger("StateStandby"), "Loaded %zu waypoints.", context.waypoints.size());

    } else {
        RCLCPP_ERROR(rclcpp::get_logger("StateStandby"), "Failed to load mission: %s", response->error_message.c_str());
        _mission_loaded = false;
    }
}

void StateStandby::on_exit(CETOContext& context) {
    // Request mission plan
}

void StateStandby::on_control_command(
    CETOContext& context,
    const ceto_interfaces::srv::FSMControl::Request& request,
    ceto_interfaces::srv::FSMControl::Response& response) {

    if(!_mission_requested && request.command == "start") {
        auto mission_plan_request = std::make_shared<ceto_interfaces::srv::LoadMission::Request>();

        RCLCPP_ERROR(rclcpp::get_logger("StateStandby"), "Requesting mission load from service...");

        mission_plan_request->mission_file_name = "test1.yaml";

        context.clientLoadMission->async_send_request(
            mission_plan_request,
            [this, &context](LoadMissionClient::SharedFuture future) {
                responseCallback(future, context);
            });
        _mission_requested = true;
    }
    response.success = true;
}

StateEnum StateStandby::get_state_id() const {
    return StateEnum::STANDBY;
}
