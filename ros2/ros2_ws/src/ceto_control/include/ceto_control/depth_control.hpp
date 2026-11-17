#include <optional>
#include <cstdint>
#include "ceto_interfaces/msg/control_setpoints.hpp"

using ControlSetpoints =  ceto_interfaces::msg::ControlSetpoints;

struct DepthInput {
    std::optional<uint8_t> depth_control_phase;

    std::optional<double> target_ballast_setting;
    std::optional<double> target_depth;
    std::optional<double> target_descent_rate;
};

struct DepthOutput {
    std::optional<uint8_t> ballast_setpoint;
};

class DepthController{
    public:
    void set_target(const ControlSetpoints& msg); //take incomming msg and set values
    void update();
    
    const DepthOutput& get_output() const {
        return depth_output_;
    }

    private:
    DepthInput depth_input_;
    DepthOutput depth_output_;

    void phase_1_(void);
    void phase_2_(void);
    void phase_3_(void);
    void phase_4_(void);
    void phase_5_(void);
    void phase_6_(void);
};