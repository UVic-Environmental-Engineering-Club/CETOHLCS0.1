# `Mission Plan Parser`

- **Package:** `mission_plan_parser`
- **Executable:** `mission_plan_parser`
- **Status:** `Working`
- **People:** `Igor`




## How to run it

The simplest way to get this node running:

**prereqs**
* `colcon build --packages-up-to mission_plan_parser`
* `source install/setup.bash`

**Command**

```bash
ros2 run mission_plan_parser mission_plan_parser
```
*Called by FSM node & integrated into launch*






## How it works

1. Runs a ROS 2 service called `/load_mission` that waits for requests from the FSM.
2. Receives the mission filename and loads the corresponding YAML file.
3. Validates the mission steps against the defined schema.
4. Converts each navigation step into a `GliderWaypoint` ROS 2 message.
5. Returns a vector of waypoints to the FSM, or an error message if validation fails.

The FSM stores the received waypoints and executes them in order.





## Important parts

* mission_parser : manages parsing, validating and storing the YAML file into `GliderWaypoint` messages
* mission_parser_node : manges ros2 service and communications
* /missions : directory where missions are stored
* mission_schema.yaml : yaml file containing current requirments for parameters that the validator checks mission plans against
 





## What has been done

* Currently Processes and validates mission plans for all phases (1-6)
* Only converts parameters into `GliderWaypoint` messages for:
    * Depth phase: 1
    * Pitch phase: 1
    * Roll phase: 1







## To do

Once phases are complete and the FSM allows for more phases update the conversion phases 







## Known issues / gotchas

* Currently does not check if values are within a given range






## Testing

How have we tested this?

* [x] Runs locally
* [ ] Runs on PI
* [ ] Integration tested
* [ ] Tested in water
* [ ] Tested on the glider
