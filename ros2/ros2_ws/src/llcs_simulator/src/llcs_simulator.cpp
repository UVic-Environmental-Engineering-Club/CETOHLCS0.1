#include "rclcpp/rclcpp.hpp"

#include "ceto_interfaces/msg/hlcs_heartbeat.hpp"
#include "ceto_interfaces/msg/llcs_heartbeat.hpp"

#define HEARTBEAT_PUBLISH_INTERVAL_MS 100
#define STARTUP_HEARTBEAT_TIMEOUT_MS 60000
#define RUN_HEARTBEAT_TIMEOUT_MS 1000

class LLCS_Simulator : public rclcpp::Node
{
    public:
        LLCS_Simulator() : Node("llcs_simulator")
        {
            _node_startup_time = this->now().nanoseconds() / 1000000; // Get the current time in milliseconds

            _last_hlcs_heartbeat_time = this->now().nanoseconds() / 1000000; // Initialize the last heartbeat time to the current time at startup giving us a one timeout period at startup.

            _publisherLLCSHeartbeat = this->create_publisher<ceto_interfaces::msg::LLCSHeartbeat>("/heartbeat/llcs", 10);

            _subscriptionHLCSHeartbeat = this->create_subscription<ceto_interfaces::msg::HLCSHeartbeat>(
                "/heartbeat/hlcs",
                10,
                std::bind(&LLCS_Simulator::handle_hlcs_heartbeat, this, std::placeholders::_1)
            );
            

            _llcs_heartbeat_timer = this->create_wall_timer(
                std::chrono::milliseconds(HEARTBEAT_PUBLISH_INTERVAL_MS),
                std::bind(&LLCS_Simulator::publish_llcs_heartbeat, this));

            _hlcs_watchdog_timer = this->create_wall_timer(
                std::chrono::milliseconds(HEARTBEAT_PUBLISH_INTERVAL_MS),
                std::bind(&LLCS_Simulator::check_hlcs_heartbeat, this));
            
        }
    
    private:
        void publish_llcs_heartbeat()
        {
            auto heartbeat_msg = ceto_interfaces::msg::LLCSHeartbeat();

            //Time in milliseconds since node startup
            heartbeat_msg.timestamp = (this->now().nanoseconds() / 1000000) - _node_startup_time; 

            heartbeat_msg.status = _llcs_status;
            heartbeat_msg.error = 0; // Set the error to indicate no error

            _publisherLLCSHeartbeat->publish(heartbeat_msg);
        }

        void handle_hlcs_heartbeat(const ceto_interfaces::msg::HLCSHeartbeat::SharedPtr msg)
        {
            // Handle the received HLCS heartbeat message
            RCLCPP_INFO(this->get_logger(), "Received HLCS Heartbeat: timestamp=%ld, status=%d", msg->timestamp, msg->status);
            _last_hlcs_heartbeat_time = this->now().nanoseconds() / 1000000; // Update the last received heartbeat time in milliseconds
            _llcs_status = 1;
        }

        void check_hlcs_heartbeat()
        {
            if (_llcs_status == 0 && (((this->now().nanoseconds() / 1000000) - _last_hlcs_heartbeat_time) > STARTUP_HEARTBEAT_TIMEOUT_MS))
            {
                RCLCPP_ERROR(this->get_logger(), "HLCS heartbeat timeout! No heartbeat received for %d ms.", STARTUP_HEARTBEAT_TIMEOUT_MS);
                // Here you can add additional actions to take when the HLCS heartbeat is lost, such as triggering a safety protocol.
            }

            if (_llcs_status == 1 && (((this->now().nanoseconds() / 1000000) - _last_hlcs_heartbeat_time) > RUN_HEARTBEAT_TIMEOUT_MS))
            {
                RCLCPP_ERROR(this->get_logger(), "HLCS heartbeat timeout! No heartbeat received for %d ms.", RUN_HEARTBEAT_TIMEOUT_MS);
                // Here you can add additional actions to take when the HLCS heartbeat is lost, such as triggering a safety protocol.
            }
        }

        rclcpp::Publisher<ceto_interfaces::msg::LLCSHeartbeat>::SharedPtr _publisherLLCSHeartbeat;
        rclcpp::TimerBase::SharedPtr _llcs_heartbeat_timer;
        rclcpp::TimerBase::SharedPtr _hlcs_watchdog_timer;
        rclcpp::Subscription<ceto_interfaces::msg::HLCSHeartbeat>::SharedPtr _subscriptionHLCSHeartbeat;
        long long _node_startup_time;
        long long _last_hlcs_heartbeat_time;

        int _llcs_status = 0; // 0: startup, 1: running, 2: error

};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LLCS_Simulator>());
    rclcpp::shutdown();
    return 0;
}