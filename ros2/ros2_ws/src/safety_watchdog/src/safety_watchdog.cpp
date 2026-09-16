#include <stdio.h>

#include "rclcpp/rclcpp.hpp"

#include "ceto_interfaces/msg/hlcs_heartbeat.hpp"
#include "ceto_interfaces/msg/llcs_heartbeat.hpp"
#include "ceto_interfaces/msg/error.hpp"

#define HEARTBEAT_PUBLISH_INTERVAL_MS 100
#define HEARTBEAT_TIMEOUT_MS 1000

class SafetyWatchdog : public rclcpp::Node
{
    public:
        SafetyWatchdog() : Node("safety_watchdog")
        {
            _node_startup_time = this->now().nanoseconds() / 1000000; // Get the current time in milliseconds

            // Initialize the last heartbeat time to the current time at startup giving us a one timeout period at startup.
            _last_llcs_heartbeat_time = this->now().nanoseconds() / 1000000;

            _publisherHLCSHeartbeat = this->create_publisher<ceto_interfaces::msg::HLCSHeartbeat>("/heartbeat/hlcs", 10);
            _publisherFatalError = this->create_publisher<ceto_interfaces::msg::Error>("/error/fatal", 10);

            _subscriptionLLCSHeartbeat = this->create_subscription<ceto_interfaces::msg::LLCSHeartbeat>(
                "/heartbeat/llcs",
                10,
                std::bind(&SafetyWatchdog::handle_llcs_heartbeat, this, std::placeholders::_1)
            );

            _hlcs_heartbeat_timer = this->create_wall_timer(
                std::chrono::milliseconds(HEARTBEAT_PUBLISH_INTERVAL_MS),
                std::bind(&SafetyWatchdog::publish_hlcs_heartbeat, this));

            _llcs_watchdog_timer = this->create_wall_timer(
                std::chrono::milliseconds(HEARTBEAT_PUBLISH_INTERVAL_MS),
                std::bind(&SafetyWatchdog::llcs_watchdog, this));
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

        void handle_llcs_heartbeat(const ceto_interfaces::msg::LLCSHeartbeat::SharedPtr msg)
        {
            // Handle the received LLCS heartbeat message
            RCLCPP_INFO(this->get_logger(), "Received LLCS Heartbeat: timestamp=%ld, status=%d", msg->timestamp, msg->status);
            _last_llcs_heartbeat_time = this->now().nanoseconds() / 1000000; // Update the last received heartbeat time in milliseconds

            if(msg->status == 2)
            {
                RCLCPP_ERROR(this->get_logger(), "LLCS reported an error! Error code: %d", msg->error);
                trigger_emergency_state("LLCS error"); //TODO: decode the llcs error code into a human readable message
            }

        }

        void llcs_watchdog()
        {
            if (((this->now().nanoseconds() / 1000000) - _last_llcs_heartbeat_time) > HEARTBEAT_TIMEOUT_MS)
            {
                RCLCPP_ERROR(this->get_logger(), "LLCS heartbeat timeout! No heartbeat received for %d ms.", HEARTBEAT_TIMEOUT_MS);
                trigger_emergency_state("LLCS heartbeat timeout");
            }
        }

        void trigger_emergency_state(std::string error_message = "Unknown safety watchdog error")
        {
            // Publish a fatal error message
            auto error_msg = ceto_interfaces::msg::Error();
            error_msg.error_message = error_message;

            _publisherFatalError->publish(error_msg);
        }

        rclcpp::Publisher<ceto_interfaces::msg::HLCSHeartbeat>::SharedPtr _publisherHLCSHeartbeat;
        rclcpp::Publisher<ceto_interfaces::msg::Error>::SharedPtr _publisherFatalError;


        rclcpp::TimerBase::SharedPtr _hlcs_heartbeat_timer;
        rclcpp::TimerBase::SharedPtr _llcs_watchdog_timer;


        rclcpp::Subscription<ceto_interfaces::msg::LLCSHeartbeat>::SharedPtr _subscriptionLLCSHeartbeat;

        long long _node_startup_time;
        long long _last_llcs_heartbeat_time;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SafetyWatchdog>());
    rclcpp::shutdown();
    return 0;
}