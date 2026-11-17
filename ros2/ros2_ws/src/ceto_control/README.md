# `ceto_control`

- **Package:** `ceto_control`
- **Executable:** `ceto_control`
- **Status:** `Phase 1 working`
- **People:** `Igor`






## How to run it

The simplest way to get this node running:

**Prereqs**
* Depth sensor connected to I2C6 port on pi hat
* `colcon build --packages-up-to ceto_control`
* `source install/setup.bash`

**Command**
```bash
ros2 run ceto_control control_node
```
*Integrated into launch*

## Sample Test (phase 1)

With ceto_control running, create 2 seperate terminals:
**Terminal 1**: to listen to actuator msg's
```bash
source install/setup.bash
ros2 topic echo /actuator/setpoints
```

**Terminal 2**: Simulate publishing a ControlSetpoints.msg from FSM
```bash
source install/setup.bash
ros2 topic pub --once /control/setpoints   ceto_interfaces/msg/ControlSetpoints   "{
    pid_mode: 1,
    depth_control_phase: 1,
    pitch_control_phase: 1,
    roll_control_phase: 1,
    target_ballast_setting: 50.0,
    target_pitch_setting: 120.0,
    target_roll_setting: 20.0
  }"
```



## How it works

1. Subscribes to `ControlSetpoints.msg`
2. Extracts the phase for each actuator, performs cooresponding
    Control calculation, and packages new values into ActuatorSetpoints.msg file
3. Publishes to `ActuatorSetpoints.msg`






## Important parts

* *control_node.cpp* :
* *depth_control.cpp/depth_control.hpp*:  Implements depth control phases and calculates ballast actuator settings.
* *pitch_control.cpp/pitch_control.hpp* :  Implements pitch control phases and calculates pitch actuator settings. 
* *roll_control.cpp/roll_control.hpp* : Implements roll control phases and calculates roll actuator settings. 
 



## What has been done
* Created the ceto_control ROS 2 package.
* Implemented the main control node and topic interfaces.
* Created separate classes for depth, pitch, and roll control.



| Phase | Description                                  | Depth Status    | Pitch Status    | Roll Status     |
| ----- | -------------------------------------------- | --------------- | --------------- | --------------- |
| 1     | direct actuator setpoint control             | Implemented     | Implemented     | Implemented     |
| 2     | Basic feedback control                       | Not implemented | Not implemented | Not implemented |
| 3     | More advanced target/rate control            | Not implemented | Not implemented | Not implemented |
| 4     | Navigation-aware control                     | Not implemented | Not implemented | Not implemented |
| 5     | Ocean current correction                     | Not implemented | Not implemented | Not implemented |
| 6     | Ocean current correction with dead reckoning | Not implemented | Not implemented | Not implemented |





## To do

* Add next phases
* Implement safe behavior for MODE_DISABLED.
* Implement emergency behavior for MODE_OVERRIDE_MAX.








## Known issues / gotchas

Things that might confuse the next person:

* `<weird behavior>`
* `<hardware limitation>`
* `<temporary workaround>`
* `<assumption the code currently makes>`

If something took you an hour to figure out, put it here so the next person doesn't have to.







## Testing

How have we tested this?

* [ ] Runs locally
* [ ] Runs on PI
* [ ] Integration tested
* [ ] Tested in water
* [ ] Tested on the glider






## Useful references

Anything that helped while working on this node:

* `<datasheet>`
* `<Paper>`
* `<documentation>`
* `<related node>`
* `<issue / design document>`






## Scratchpad / random stuff

This section is intentionally unstructured.


* `<random useful thing>`
* `<questions>`
* `<idea to try later>`