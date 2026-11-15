#include <string>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "ceto_interfaces/msg/glider_waypoint.hpp"

#pragma once 

const char* DIRECTORY = "/ros2/ros2_ws/src/mission_plan_parser/missions/";
const char* SCHEMA_DIRECTORY = "/ros2/ros2_ws/src/mission_plan_parser/mission_schema.yaml";

class MissionParser{
    private:
    std::string mission_file_name;
    std::string filepath;
    YAML::Node parse_result;

    YAML::Node steps;
    size_t step_count;
    ceto_interfaces::msg::GliderWaypoint waypoint;
    std::vector<ceto_interfaces::msg::GliderWaypoint>waypoints;

    public:
    MissionParser(std::string mission_file_name);
    void to_waypoints();
    int validate_mission();

};