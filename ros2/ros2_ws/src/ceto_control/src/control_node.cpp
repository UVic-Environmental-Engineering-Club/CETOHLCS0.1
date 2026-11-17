#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "ceto_interfaces/msg/control_setpoints.hpp"
#include "ceto_interfaces/msg/actuator_setpoints.hpp"

#include "ceto_control/depth_control.hpp"
#include "ceto_control/pitch_control.hpp"
#include "ceto_control/roll_control.hpp"

using ControlSetpoints =  ceto_interfaces::msg::ControlSetpoints;
using ActuatorSetpoints = ceto_interfaces::msg::ActuatorSetpoints;

class CetoControl : public rclcpp::Node
{
public:
    CetoControl() : Node("ceto_control")
    {
        // subscribe to control/setpoints from FSM
        control_subscription_ =
            this->create_subscription<ControlSetpoints>(
                "/control/setpoints",
                10,
                [this](ControlSetpoints::SharedPtr msg) {
                    on_setpoints(*msg);
                }
            );
        actuator_publisher_ =
        this->create_publisher<ActuatorSetpoints>(
        "/actuator/setpoints",
        10
    );
    }
    
private:
    rclcpp::Subscription<ControlSetpoints>::SharedPtr control_subscription_;
    rclcpp::Publisher<ActuatorSetpoints>::SharedPtr actuator_publisher_;

    ControlSetpoints current_setpoints_{};

    DepthController depth_controller_;
    PitchController pitch_controller_;
    RollController roll_controller_;

    void on_setpoints(const ControlSetpoints& msg)
    {
        current_setpoints_ = msg;

        depth_controller_.set_target(current_setpoints_);
        pitch_controller_.set_target(current_setpoints_);
        roll_controller_.set_target(current_setpoints_);

        depth_controller_.update();
        pitch_controller_.update();
        roll_controller_.update();

        ActuatorSetpoints command;

        const DepthOutput depth_output = depth_controller_.get_output();
        const PitchOutput pitch_output = pitch_controller_.get_output();
        const RollOutput roll_output = roll_controller_.get_output();

        command.ballast_setpoint = depth_output.ballast_setpoint.value();
        command.pitch_setpoint = pitch_output.pitch_setpoint.value();
        command.roll_setpoint = roll_output.roll_setpoint.value();

        actuator_publisher_->publish(command);
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CetoControl>());
    rclcpp::shutdown();
    return 0;
}
