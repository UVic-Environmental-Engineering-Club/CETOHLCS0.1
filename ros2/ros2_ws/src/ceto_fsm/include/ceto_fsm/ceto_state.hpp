#pragma once

#include <vector>
#include <memory>
#include <string>
// #include "glider_interfaces/msg/glider_waypoint.hpp"
// #include "glider_interfaces/msg/control_setpoints.hpp" 

// 1. State Enumeration
// Matches the specific FSM states defined in the mission architecture.
enum class StateEnum {
    ON,             // System bootup and calibration
    STANDBY,        // Waiting for input/time, reporting status
    OFF,            // Save data, sequential power down
    DIVE_DECENT,    // Active navigation to waypoint (diving)
    DIVE_ACCENT,    // Active navigation to waypoint (surfacing)
    SURFACE,        // Arrived at surface waypoint, waiting/reporting
    EMERGENCY,      // Hardware or software failure detected 
};

// 2. The Blackboard (CETOContext)
// This holds all data parsed from the sensors, dead reckoning, and mission files.
// It is passed by reference to every state to avoid redundant ROS 2 subscriptions.
struct CETOContext {
    // Mission Planning
    std::vector<glider_interfaces::msg::GliderWaypoint> mission_plan;
    size_t current_waypoint_index = 0;
};

// 3. The Abstract Base Class
class CETOState {
public:
    virtual ~CETOState() = default;

    // Triggered exactly once when the FSM transitions into this state.
    // Use this to reset internal counters, log state entry, or send one-off commands.
    virtual void on_enter(CETOContext& context) = 0;

    // Triggered continuously by the main FSM timer tick (e.g., 10 Hz).
    // Calculates and returns the target setpoints to be published to the PID nodes.
    [[nodiscard]] virtual glider_interfaces::msg::ControlSetpoints execute(CETOContext& context) = 0;

    // Evaluates conditions on every tick to determine if a transition should occur.
    // Returns the same state ID if no transition is needed, or a new state ID to trigger a change.
    [[nodiscard]] virtual StateEnum check_transitions(CETOContext& context) = 0;

    // Triggered exactly once when leaving the state.
    // Use this to clean up, save data, or explicitly disable PID controllers before swapping states.
    virtual void on_exit(CETOContext& context) = 0;
    
    // Returns the identifier for the specific implementation.
    [[nodiscard]] virtual StateEnum get_state_id() const = 0;
};