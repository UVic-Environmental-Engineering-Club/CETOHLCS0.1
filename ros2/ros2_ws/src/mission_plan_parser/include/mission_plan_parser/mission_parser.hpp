#include <string>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "ceto_interfaces/msg/glider_waypoint.hpp"

#pragma once 

using waypoint_msg_t = ceto_interfaces::msg::GliderWaypoint;

class MissionParser{
    public:

    MissionParser(std::string mission_file_name);
    int validate_mission();
    waypoint_msg_t to_waypoint(YAML::Node navigation_step);

    const std::vector<waypoint_msg_t>& get_waypoints() const 
    {
        return waypoints;
    }

    private:
    std::string mission_file_name;
    std::string filepath;
    YAML::Node parse_result;

    YAML::Node steps;
    size_t step_count;

    std::vector<waypoint_msg_t>waypoints;
};