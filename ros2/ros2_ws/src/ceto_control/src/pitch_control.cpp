#include "ceto_control/pitch_control.hpp"

void PitchController::set_target(const ControlSetpoints& msg)
{
    pitch_input_ = {};//clear away old values

    pitch_input_.pitch_control_phase = msg.pitch_control_phase;

    pitch_input_.target_pitch = msg.target_pitch;
    pitch_input_.target_pitch_setting = msg.target_pitch_setting;

    return;
}

void PitchController::update(void)
{
    pitch_output_ = {};//clear away old values
    
    if (!pitch_input_.pitch_control_phase.has_value()) 
    {
        return;
    }

    switch (pitch_input_.pitch_control_phase.value())
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
phase_x_() executes phase x of pitch control
on pitch_input_ into pitch_output_
==================================================== */
void PitchController::phase_1_(void)
{
    pitch_output_.pitch_setpoint = static_cast<uint8_t>(pitch_input_.target_pitch_setting.value());
}

void PitchController::phase_2_(void)
{
    
}

void PitchController::phase_3_(void)
{
    
}

void PitchController::phase_4_(void)
{
    
}

void PitchController::phase_5_(void)
{
    
}

void PitchController::phase_6_(void)
{
    
}