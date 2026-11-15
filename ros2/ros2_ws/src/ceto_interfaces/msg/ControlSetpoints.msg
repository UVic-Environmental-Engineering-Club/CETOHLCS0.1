# The glider control setpoints

# std_msgs must be added to your package.xml and CMakeLists.txt dependencies
#std_msgs/Header header

# PID Operation Modes 
# Defining constants here allows you to use ControlSetpoints::MODE_ACTIVE directly in C++
uint8 MODE_DISABLED = 0       # PID loops idle, hardware holds current state
uint8 MODE_ACTIVE = 1         # PID loops actively chasing targets
uint8 MODE_OVERRIDE_MAX = 2   # Emergency mode (e.g., max buoyancy, masses full forward)

# Global PID state dictated by the FSM
uint8 pid_mode

# Target Setpoints (for PID loops)
float64 target_depth
float64 target_decent_rate
float64 target_pitch
float64 target_roll

# Actuactor Setpoints (for pre-PID control systems)
float64 target_pitch_setting
float64 target_ballast_setting
float64 target_roll_setting

# Active Control Phases 
# Maps directly to the 1-6 development phases for each control subsystem
uint8 depth_control_phase
uint8 pitch_control_phase
uint8 roll_control_phase