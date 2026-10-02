#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "ceto_fsm/ceto_state.hpp"
#include "ceto_fsm/states/state_on.hpp"
#include "ceto_fsm/states/state_standby.hpp"

#include "ceto_interfaces/msg/llcs_heartbeat.hpp"


#define TICK_INTERVAL_MS 100 // 10 Hz

class CETOFSM : public rclcpp::Node
{
    public:
        CETOFSM() : Node("ceto_fsm")
        {
            
            _context = CETOContext();
            
            _current_state = std::make_unique<StateOn>();
            _current_state->on_enter(_context);

            _subscriptionLLCSHeartbeat = this->create_subscription<ceto_interfaces::msg::LLCSHeartbeat>(
                "/heartbeat/llcs",
                10,
                std::bind(&CETOFSM::handle_llcs_heartbeat, this, std::placeholders::_1)
            );

            _publisherControlSetpoints = this->create_publisher<ceto_interfaces::msg::ControlSetpoints>(
                "/control/setpoints",
                10
            );

            _tick_timer = this->create_wall_timer(
                std::chrono::milliseconds(TICK_INTERVAL_MS), // 10 Hz
                std::bind(&CETOFSM::tick, this));
        }
    
    private:
        rclcpp::TimerBase::SharedPtr _tick_timer;

        CETOContext _context;
        std::unique_ptr<CETOState> _current_state;
        ceto_interfaces::msg::ControlSetpoints _setpoints;

        rclcpp::Subscription<ceto_interfaces::msg::LLCSHeartbeat>::SharedPtr _subscriptionLLCSHeartbeat;

        rclcpp::Publisher<ceto_interfaces::msg::ControlSetpoints>::SharedPtr _publisherControlSetpoints;


        void tick()
        {
            
            // This function will be called at 10 Hz
            // Here you would typically call the execute method of the current state
            // and check for transitions to other states.
            _setpoints = _current_state->execute(_context);
            _publisherControlSetpoints->publish(_setpoints);

            const StateEnum next_state = _current_state->check_transitions(_context);
            if (next_state != _current_state->get_state_id())
            {
                auto next = create_state(next_state);
                if (next) {
                    _current_state->on_exit(_context);
                    _current_state = std::move(next);
                    _current_state->on_enter(_context);
                } else {
                    RCLCPP_ERROR(
                        this->get_logger(),
                        "No implementation for requested state: %d",
                        static_cast<int>(next_state));
                }
            }

            RCLCPP_INFO(this->get_logger(), "Current State: %d", static_cast<int>(_current_state->get_state_id()));

        }

        void handle_llcs_heartbeat(const ceto_interfaces::msg::LLCSHeartbeat::SharedPtr msg)
        {
            _context.stm32_state = msg->state;
        }

        std::unique_ptr<CETOState> create_state(StateEnum state)
        {
            switch (state) {
                case StateEnum::ON:
                    return std::make_unique<StateOn>();
                case StateEnum::STANDBY:
                    return std::make_unique<StateStandby>();
                default:
                    return nullptr;  // No implementation exists yet for this state.
            }
        }

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CETOFSM>());
    rclcpp::shutdown();
    return 0;
}