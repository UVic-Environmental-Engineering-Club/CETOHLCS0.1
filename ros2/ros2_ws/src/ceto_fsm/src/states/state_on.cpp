#include "ceto_fsm/states/state_on.hpp"
#include "ceto_interfaces/msg/glider_waypoint.hpp"

#define STANDBY_PITCH_SETTING 0.0
#define STANDBY_ROLL_SETTING 0.0
#define STANDBY_BALLAST_SETTING 10.0

ceto_interfaces::msg::ControlSetpoints StateOn::on_enter(CETOContext& context) {

    //Placeholder for fetching waypoints from the mission plan file.
    std::vector<ceto_interfaces::msg::GliderWaypoint> waypoints = { 
        ceto_interfaces::msg::GliderWaypoint()
    };
    waypoints[0].standby_time = 5000;
    
    waypoints[0].depth_phase = 1;
    waypoints[0].pitch_phase = 1;
    waypoints[0].roll_phase = 1;

    waypoints[0].decent_ballast_setting = 60.0;
    waypoints[0].decent_pitch_setting = 10.0;
    waypoints[0].decent_roll_setting = 0;
    waypoints[0].decent_time = 10000;

    waypoints[0].accent_ballast_setting = 40.0;
    waypoints[0].accent_pitch_setting = 0.0;
    waypoints[0].accent_roll_setting = 0.0;
    waypoints[0].accent_time = 10000;


    context.waypoints = waypoints;

    ceto_interfaces::msg::ControlSetpoints setpoints;

    setpoints.pid_mode = ceto_interfaces::msg::ControlSetpoints::MODE_DISABLED;

    return setpoints;

}

StateEnum StateOn::check_transitions(CETOContext& context) {
    // Implementation for checking transitions from the ON state
    // Return the next state based on conditions

    if(context.stm32_state == 1 && context.waypoints.size() > 0) {
        return StateEnum::STANDBY;
    }

    return StateEnum::ON;
}

void StateOn::on_exit(CETOContext& context) {
    // Implementation for exiting the ON state
}

StateEnum StateOn::get_state_id() const {
    return StateEnum::ON;
}