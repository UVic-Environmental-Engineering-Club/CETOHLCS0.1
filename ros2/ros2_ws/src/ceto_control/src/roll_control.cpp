#include "ceto_control/roll_control.hpp"

void RollController::set_target(const ControlSetpoints& msg)
{
    roll_input_ = {};//clear away old values
    
    roll_input_.roll_control_phase = msg.roll_control_phase;

    roll_input_.target_roll = msg.target_roll;
    roll_input_.target_roll_setting = msg.target_roll_setting;

    return;
}

void RollController::update(void)
{
    roll_output_ = {};//clear away old values

    if (!roll_input_.roll_control_phase.has_value()) 
    {
        return;
    }

    switch (roll_input_.roll_control_phase.value())
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
phase_x_() executes phase x of roll control
on roll_input_ into roll_output_
==================================================== */
void RollController::phase_1_(void)
{
    roll_output_.roll_setpoint = static_cast<uint8_t>(roll_input_.target_roll_setting.value());
}

void RollController::phase_2_(void)
{
    
}

void RollController::phase_3_(void)
{
    
}

void RollController::phase_4_(void)
{
    
}

void RollController::phase_5_(void)
{
    
}

void RollController::phase_6_(void)
{
    
}