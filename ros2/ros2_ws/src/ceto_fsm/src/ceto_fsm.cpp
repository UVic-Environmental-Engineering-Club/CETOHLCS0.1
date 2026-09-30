#include <stdio.h>

#include "rclcpp/rclcpp.hpp"

#include "ceto_fsm/ceto_fsm_state.hpp"
#include "ceto_fsm/states/state_on.hpp"

#include 


#define TICK_INTERVAL_MS 100 // 10 Hz

class CETOFSM : public rclcpp::Node
{
    public:
        CETOFSM() : Node("ceto_fsm")
        {
            
            _context = CETOContext();
            
            _current_state = std::make_unique<StateOn>();
            _current_state->on_enter(_context);

            _tick_timer = this->create_wall_timer(
                std::chrono::milliseconds(TICK_INTERVAL_MS), // 10 Hz
                std::bind(&CETOFSM::tick, this));
        }
    
    private:
        rclcpp::TimerBase::SharedPtr _tick_timer;

        CETOContext _context;
        std::unique_ptr<CETOState> _current_state = std::make_unique<CETOState>();

        # Subscriptions



        void tick()
        {
            // This function will be called at 10 Hz
            // Here you would typically call the execute method of the current state
            // and check for transitions to other states.


        }

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CETOFSM>());
    rclcpp::shutdown();
    return 0;
}