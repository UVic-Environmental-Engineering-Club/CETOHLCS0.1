#include "mission_plan_parser/mission_parser.hpp"

MissionParser::MissionParser(std::string mission_file_name)
: mission_file_name(mission_file_name){
    filepath = DIRECTORY + mission_file_name;
    try{
        parse_result =  YAML::LoadFile(filepath);
        std::cout << "Mission loaded successfully!\n"; 
        
        waypoints = parse_result["waypoints"];
        waypoint_count = waypoints.size();

    }catch (const YAML::Exception &e){
        std::cerr << e.what() << "\n";
        throw;
    }catch (const std::exception &e){
        std::cerr << e.what() << "\n";
        throw;
    }
}


int MissionParser::validate_mission()
{
    YAML::Node schema;
    try{
        schema = YAML::LoadFile(SCHEMA_DIRECTORY);
    } catch(const YAML::Exception &e) {
        std::cerr << e.what() << "\n";
        throw;
    }

    for (size_t i = 0; i < waypoint_count; i++) {
        YAML::Node waypoint_node = waypoints[i];

        for (const auto& section : {"depth", "pitch", "roll"}) {
            YAML::Node section_node = waypoint_node[section];

            int phase = section_node["phase"].as<int>();

            YAML::Node required = schema[section]["phase"][phase];

            if (!required) {
                std::cerr << "Unknown phase " << phase << " for " << section << "\n";
                return 1;
            }

            //check number of parameters (+1 for "phase")
            if (section_node.size() != required.size() + 1) {
                std::cerr << "Incorrect number of parameters in " << section << " at waypoint: " << waypoint_node["id"].as<std::string>() << "\n";
                return 1;
            }

            // 2. check every required parameter exists
            for (const auto& parameter : required) {
                std::string name = parameter.as<std::string>();

                if (!section_node[name]) {
                    std::cerr << "Missing parameter: " << name << " at waypoint: " << waypoint_node["id"].as<std::string>() << "\n";
                    return 1;
                }
            }
        }
    }

    return 0;
}


int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;

    if (argc < 2) {
        std::cerr << "Usage: program <mission.yaml>\n";
        return 1;
    }
    try {
        MissionParser mission_parser(argv[1]);
        mission_parser.validate_mission();
    } catch (const std::exception& e) {
        return 1;
    }

    
    return 0;
}
