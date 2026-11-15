#Heartbeat message sent from the LLCS(STM32) to the HLCS(Pi)

# Timestamp as milliseconds since the LLCS started
uint64 timestamp

# State of the LLCS (0 = setup, 1 = run, 2 = emergency)
uint8 state

# Error Code (0 if no error, other codes TBD)
uint8 error