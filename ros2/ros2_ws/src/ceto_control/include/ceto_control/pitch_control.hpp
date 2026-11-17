#include <optional>
#include <cstdint>
#include "ceto_interfaces/msg/control_setpoints.hpp"

using ControlSetpoints =  ceto_interfaces::msg::ControlSetpoints;

/**
 * @brief Stores target values received from the FSM.
 */
struct PitchInput {
    std::optional<uint8_t> pitch_control_phase;

    std::optional<double> target_pitch;
    std::optional<double> target_pitch_setting;
};

/**
 * @brief Stores target values to be outputed to the actuators.
 */
struct PitchOutput {
    std::optional<uint8_t> pitch_setpoint;
};

/**
 * @brief Controls gliders buoyancy based on the selected
 *        depth control phase.
 */
class PitchController{
    public:
    void set_target(const ControlSetpoints& msg);
    void update();

    const PitchOutput& get_output() const {
        return pitch_output_;
    }

    private:
    PitchInput pitch_input_;
    PitchOutput pitch_output_;

    void phase_1_(void);
    void phase_2_(void);
    void phase_3_(void);
    void phase_4_(void);
    void phase_5_(void);
    void phase_6_(void);
};