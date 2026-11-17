#include <optional>
#include <cstdint>
#include "ceto_interfaces/msg/control_setpoints.hpp"

using ControlSetpoints =  ceto_interfaces::msg::ControlSetpoints;

/**
 * @brief Stores target values received from the FSM.
 */
struct RollInput {
    std::optional<uint8_t> roll_control_phase;

    std::optional<double> target_roll;
    std::optional<double> target_roll_setting;
};

/**
 * @brief Stores target values to be outputed to the actuators.
 */
struct RollOutput {
    std::optional<uint8_t> roll_setpoint;
};

/**
 * @brief Controls gliders buoyancy based on the selected
 *        depth control phase.
 */
class RollController{
    public:
    void set_target(const ControlSetpoints& msg);
    void update();

    const RollOutput& get_output() const {
        return roll_output_;
    }

    private:
    RollInput roll_input_;
    RollOutput roll_output_;

    void phase_1_(void);
    void phase_2_(void);
    void phase_3_(void);
    void phase_4_(void);
    void phase_5_(void);
    void phase_6_(void);
};