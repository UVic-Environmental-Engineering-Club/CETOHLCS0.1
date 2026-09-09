#Heartbeat message sent from the HLCS(Pi) to the LLCS(STM32)

# Timestamp as milliseconds since the HLCS safety watchdog node was started
uint64 timestamp

# Status of the HLCS (0 = setup, 1 = run, 2 = emergency)
uint8 status
