#include "mission_plan_parser/mission_parser.hpp"

MissionParser::MissionParser(std::string mission_file_name)
: mission_file_name(mission_file_name){
    filepath = DIRECTORY + mission_file_name;
    try{
        parse_result =  YAML::LoadFile(filepath);
        std::cout << "Mission loaded successfully!\n"; 
        
        steps = parse_result["steps"];
        step_count = steps.size();

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

    for (size_t i = 0; i < step_count; i++) {
        YAML::Node step = steps[i];

        if (!step["type"]) {
            std::cerr << "Step " << step["id"].as<std::string>() << " missing type\n";
            return 1;
        }
        std::string step_type = step["type"].as<std::string>(); 
        std::string step_id = step["id"].as<std::string>();

        //Type = navigation -----------------------------------------------------------------
        if (step_type == "navigation"){
            if (!step["standby_time_s"]) { //check for standby time
                std::cerr << "No standby_time_s provided for step:" << step["id"].as<std::string>() << "\n";
                return 1;
            }
            for (const auto& section : {"depth", "pitch", "roll"}) {
                YAML::Node section_node = step[section];

                int phase = section_node["phase"].as<int>();

                YAML::Node required = schema[section]["phase"][phase];
                if (!required) { //check if phase exists
                    std::cerr << "Unknown phase " << phase << " for " << section << "\n";
                    return 1;
                }

                //check if location data is needed
                bool needs_latitude = false;
                bool needs_longitude = false;

                for (const auto& parameter : required) {
                    std::string name = parameter.as<std::string>();

                    if (name == "latitude")
                        needs_latitude = true;

                    if (name == "longitude")
                        needs_longitude = true;
                }

                if(needs_longitude && needs_latitude){
                    //check if longitude defined in step
                    if(!step["location"]["latitude"] || !step["location"]["longitude"]){
                        std::cerr << "Latitude and Longitude needed for phase " << phase <<" of "<< section << ", but not properly defined (at step: " << step_id << ")\n";
                        return 1;
                    }else{
                        //if they are defined add them into the yaml node
                        section_node["latitude"] = step["location"]["latitude"];
                        section_node["longitude"] = step["location"]["longitude"];
                    }
                }

                //check number of parameters (+1 for "phase")
                if (section_node.size() != required.size() + 1) {
                    std::cerr << "Incorrect number of parameters in " << section << " at step: " << step_id << "\n";
                    return 1;
                }

                // 2. check every required parameter exists
                for (const auto& parameter : required) {
                    std::string parameter_name = parameter.as<std::string>();

                    if (!section_node[parameter_name]) {
                        std::cerr << "Missing parameter: " << parameter_name << " at step: " << step_id << "\n";
                        return 1;
                    }
                }
            }
            //package and send nav data
        }
        //Type Unkown -----------------------------------------------------------------
        else{
            std::cerr << "Unknown step type" << '\n';
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
        std::cerr << "Mission parsing failed: " << e.what() << "\n";
        return 1;
    }

    
    return 0;
}
