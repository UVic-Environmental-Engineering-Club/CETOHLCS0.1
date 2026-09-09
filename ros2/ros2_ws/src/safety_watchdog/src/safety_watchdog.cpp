#include "rclcpp/rclcpp.hpp"

#include "ceto_interfaces/msg/hlcs_heartbeat.hpp"
#include "ceto_interfaces/msg/llcs_heartbeat.hpp"

class SafetyWatchdog : public rclcpp::Node
{
    public:
        SafetyWatchdog() : Node("safety_watchdog")
        {
            _node_startup_time = this->now().nanoseconds() / 1000000; // Get the current time in milliseconds

            _publisherHLCSHeartbeat = this->create_publisher<ceto_interfaces::msg::HLCSHeartbeat>("/heartbeat/hlcs", 10);

            _hlcs_heartbeat_timer = this->create_wall_timer(
                std::chrono::milliseconds(100),
                std::bind(&SafetyWatchdog::publish_hlcs_heartbeat, this));
        }
    
    private:
        void publish_hlcs_heartbeat()
        {
            auto heartbeat_msg = ceto_interfaces::msg::HLCSHeartbeat();

            //Time in milliseconds since node startup
            heartbeat_msg.timestamp = (this->now().nanoseconds() / 1000000) - _node_startup_time; 

            heartbeat_msg.status = 1; // Set the status to indicate the node is alive

            _publisherHLCSHeartbeat->publish(heartbeat_msg);


        }

        rclcpp::Publisher<ceto_interfaces::msg::HLCSHeartbeat>::SharedPtr _publisherHLCSHeartbeat;
        rclcpp::TimerBase::SharedPtr _hlcs_heartbeat_timer;

        long long _node_startup_time;

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SafetyWatchdog>());
    rclcpp::shutdown();
    return 0;
}