# Start standby time (ms)
uint64 start_standby_time

# End standby time (ms)
uint64 end_standby_time

# Control Phases
uint8 depth_phase
uint8 pitch_phase
uint8 roll_phase

# Phase 1 parameters
uint64 decent_time # (ms)
uint64 accent_time # (ms)

uint8 decent_ballast_setting # (%)
uint8 decent_pitch_setting # (mm)
uint8 decent_roll_setting # (degrees)

uint8 accent_ballast_setting # (%)
uint8 accent_pitch_setting # (mm)
uint8 accent_roll_setting # (degrees)