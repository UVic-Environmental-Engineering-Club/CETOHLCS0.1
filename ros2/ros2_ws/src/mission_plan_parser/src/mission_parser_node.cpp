#include "mission_plan_parser/mission_parser.hpp"
#include "rclcpp/rclcpp.hpp"
#include "ceto_interfaces/srv/load_mission.hpp"
#include <memory>

class MissionParserNode : public rclcpp::Node {
public:
    MissionParserNode() : Node("mission_parser") {

        service_ = create_service<ceto_interfaces::srv::LoadMission>(
            "load_mission",

            [](
                std::shared_ptr<ceto_interfaces::srv::LoadMission::Request> request,
                std::shared_ptr<ceto_interfaces::srv::LoadMission::Response> response
            ){
                try {
                    //Load the requested YAML file
                    MissionParser parser(request->mission_file_name);

                    //Validate it
                    if (parser.validate_mission() != 0) {
                        response->success = false;
                        response->error_message = "Invalid mission";
                        return;
                    }

                    //send waypoint vector
                    response->waypoints = parser.get_waypoints();
                    response->success = true;

                } catch (const std::exception& e) {
                    response->success = false;
                    response->error_message = e.what();
                }
            }
        );
    }

private:
    rclcpp::Service<ceto_interfaces::srv::LoadMission>::SharedPtr service_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<MissionParserNode>());

    rclcpp::shutdown();
    return 0;
}
