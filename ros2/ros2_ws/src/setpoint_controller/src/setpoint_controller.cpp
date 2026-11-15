#include "rclcpp/rclcpp.hpp"
#include "ceto_interfaces/msg/control_setpoints.hpp"
#include "ceto_interfaces/msg/actuator_setpoints.hpp"

#define TICK_INTERVAL_MS 100 // 10 Hz

class SetpointController : public rclcpp::Node
{
  public:

      SetpointController() : Node("setpoint_controller")
      {
          _subscriptionControlSetpoints = this->create_subscription<ceto_interfaces::msg::ControlSetpoints>(
              "/control/setpoints",
              10,
              std::bind(&SetpointController::handle_control_setpoints, this, std::placeholders::_1)
          );
          
          _publisherActuatorSetpoints = this->create_publisher<ceto_interfaces::msg::ActuatorSetpoints>(
              "/actuator/setpoints",
              10
          );

          _tick_timer = this->create_wall_timer(
              std::chrono::milliseconds(TICK_INTERVAL_MS), // 10 Hz
              std::bind(&SetpointController::tick, this));

      }

  
  private:
      
    void handle_control_setpoints(const ceto_interfaces::msg::ControlSetpoints::SharedPtr msg)
    {
        _current_setpoints = *msg;
    }

    void tick()
    {
        // This function will be called at 10 Hz
        // Here you would typically call the execute method of the current state

        ceto_interfaces::msg::ActuatorSetpoints actuator_setpoints_msg;

        actuator_setpoints_msg.ballast_setpoint = calculate_ballast_setting();
        actuator_setpoints_msg.pitch_setpoint = calculate_pitch_setting();
        actuator_setpoints_msg.roll_setpoint = calculate_roll_setting();

        _publisherActuatorSetpoints->publish(actuator_setpoints_msg);
    }

    int calculate_ballast_setting()
    {
      if (_current_setpoints.depth_control_phase == 1)
      {
          return _current_setpoints.target_ballast_setting;
      }
      return 0;
    }

    int calculate_pitch_setting()
    {
      if (_current_setpoints.depth_control_phase == 1)
      {
          return _current_setpoints.target_pitch_setting;
      }
      return 0;
    }

    int calculate_roll_setting()
    {
      if (_current_setpoints.depth_control_phase == 1)
      {
          return _current_setpoints.target_roll_setting;
      }
      return 0;
    }

    rclcpp::Publisher<ceto_interfaces::msg::ControlSetpoints>::SharedPtr _publisherControlSetpoints;
    
    rclcpp::Subscription<ceto_interfaces::msg::ControlSetpoints>::SharedPtr _subscriptionControlSetpoints;
    rclcpp::Publisher<ceto_interfaces::msg::ActuatorSetpoints>::SharedPtr _publisherActuatorSetpoints;

    rclcpp::TimerBase::SharedPtr _tick_timer;

    ceto_interfaces::msg::ControlSetpoints _current_setpoints;

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SetpointController>());
    rclcpp::shutdown();
    return 0;
}