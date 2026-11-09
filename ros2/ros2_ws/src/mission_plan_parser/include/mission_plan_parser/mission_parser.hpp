#include <string>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <yaml-cpp/yaml.h>

#pragma once 

const char* DIRECTORY = "/ros2/ros2_ws/src/mission_plan_parser/missions/";
const char* SCHEMA_DIRECTORY = "/ros2/ros2_ws/src/mission_plan_parser/mission_schema.yaml";

class MissionParser{
    private:
    std::string mission_file_name;
    std::string filepath;
    YAML::Node parse_result;

    YAML::Node waypoints;
    size_t waypoint_count;

    public:
    MissionParser(std::string mission_file_name);

    int validate_mission();

};