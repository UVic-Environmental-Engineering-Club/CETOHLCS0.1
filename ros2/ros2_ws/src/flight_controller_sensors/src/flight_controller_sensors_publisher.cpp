#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

#include "bindings.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


class FlightControllerSensorsPublisher : public rclcpp::Node
{
    public:
        FlightControllerSensorsPublisher() : Node("flight_controller_sensors_publisher")
        {
            publisher_ = this->create_publisher<example_interfaces::msg::String>("string_test_topic", 10);

            
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(1000),
                std::bind(&FlightControllerSensorsPublisher::publish_sensor_data, this));
            
            _couter = 0;

            RCLCPP_INFO(this->get_logger(), "Initiating navigator module.\n");
            init();

            RCLCPP_INFO(this->get_logger(), "Setting led on!\n");
            set_led(UserLed::Led1, true);

            RCLCPP_INFO(this->get_logger(), "Temperature: %f\n", read_temp());
            RCLCPP_INFO(this->get_logger(), "Pressure: %f\n", read_pressure());

            AxisData mag = read_mag();
            RCLCPP_INFO(this->get_logger(), "Magnetic field: X = %f, Y = %f, Z = %f\n", mag.x, mag.y, mag.z);

        }
    
    private:

        void publish_sensor_data()
        {
            auto message = example_interfaces::msg::String();
            message.data = "Hello, this is a test message from the flight controller sensors publisher with lib import! Count: " + std::to_string(_couter);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            publisher_->publish(message);
            _couter++;
        }    

        rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        size_t _couter;
};

int main(int argc, char * argv[])
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<FlightControllerSensorsPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

