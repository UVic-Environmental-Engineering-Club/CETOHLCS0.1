#include "rclcpp/rclcpp.hpp"

#include "ceto_interfaces/msg/hlcs_heartbeat.hpp"
#include "ceto_interfaces/msg/llcs_heartbeat.hpp"

class LLCS_Simulator : public rclcpp::Node
{
    public:
        LLCS_Simulator() : Node("llcs_simulator")
        {
            _node_startup_time = this->now().nanoseconds() / 1000000; // Get the current time in milliseconds

            _publisherLLCSHeartbeat = this->create_publisher<ceto_interfaces::msg::LLCSHeartbeat>("/heartbeat/llcs", 10);

            _hlcs_heartbeat_timer = this->create_wall_timer(
                std::chrono::milliseconds(100),
                std::bind(&LLCS_Simulator::publish_llcs_heartbeat, this));
        }
    
    private:
        void publish_llcs_heartbeat()
        {
            auto heartbeat_msg = ceto_interfaces::msg::LLCSHeartbeat();

            //Time in milliseconds since node startup
            heartbeat_msg.timestamp = (this->now().nanoseconds() / 1000000) - _node_startup_time; 

            heartbeat_msg.status = 1; // Set the status to indicate the node is alive
            heartbeat_msg.error = 0; // Set the error to indicate no error

            _publisherLLCSHeartbeat->publish(heartbeat_msg);
        }

        rclcpp::Publisher<ceto_interfaces::msg::LLCSHeartbeat>::SharedPtr _publisherLLCSHeartbeat;
        rclcpp::TimerBase::SharedPtr _hlcs_heartbeat_timer;

        long long _node_startup_time;

};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LLCS_Simulator>());
    rclcpp::shutdown();
    return 0;
}