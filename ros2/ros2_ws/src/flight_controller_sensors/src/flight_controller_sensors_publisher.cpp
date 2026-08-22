#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"


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

        }
    
    private:

        void publish_sensor_data()
        {
            auto message = example_interfaces::msg::String();
            message.data = "Hello, this is a test message from the flight controller sensors publisher! Count: " + std::to_string(_couter);
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

