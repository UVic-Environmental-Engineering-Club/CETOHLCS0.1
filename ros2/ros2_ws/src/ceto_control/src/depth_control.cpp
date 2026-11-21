
#include "ceto_control/depth_control.hpp"

void DepthController::set_target(const ControlSetpoints& msg)
{
    depth_input_ = {};//clear away old values

    depth_input_.depth_control_phase = msg.depth_control_phase;

    depth_input_.target_depth = msg.target_depth;
    depth_input_.target_descent_rate = msg.target_descent_rate;
    depth_input_.target_ballast_setting = msg.target_ballast_setting;

    return;
}

void DepthController::update(void)
{
    depth_output_ = {};//clear away old values

    if (!depth_input_.depth_control_phase.has_value()) 
    {
        return;
    }

    switch (depth_input_.depth_control_phase.value())
    {
        case(1):
            phase_1_();
            break;
        case(2):
            phase_2_();
            break;
        case(3):
            phase_3_();
            break;
        case(4):
            phase_4_();
            break;
        case(5):
            phase_5_();
            break;
        case(6):
            phase_6_();
            break;
        default:
            //report unsupported case
            break;
        
    } 
}

/* ====================================================
phase_x_() executes phase x of depth control
on depth_input_ into depth_output_
==================================================== */
void DepthController::phase_1_(void)// Descend for a fixed amount of time
{
    depth_output_.ballast_setpoint =  static_cast<uint8_t>(depth_input_.target_ballast_setting.value());
}

void DepthController::phase_2_(void)// Descend to a target depth with set ballast settings
{
    
}

void DepthController::phase_3_(void)// Descend and Ascend to and from a target depth at a fixed rate
{
    
}

void DepthController::phase_4_(void)// Fixed decent and accent decent rate settings calculated based on distance to waypoint
{
    
}

void DepthController::phase_5_(void)// Fixed decent and accent decent rate settings calculated based on distance to waypoint with ocean current correction
{
    
}

void DepthController::phase_6_(void)// Fixed decent and accent decent rate settings calculated based on distance to waypoint with ocean current correction and dead reckoning
{
    
}