# `Flight Controller Sensors`

- **Package:** `flight_controller_sensors`
- **Executable:** `flight_controller_sensors_publisher`
- **Status:** `In progress`
- **People:** `Sebastien`






## How to run it

The simplest way to get this node running:

**prereqs**
* `<dependency / hardware / another node>`
* `colcon build`
* `source install/setup.bash`

**Command**

```bash
ros2 ros2 run flight_controller_sensors flight_controller_sensors_publisher
```







## How it works

Describe the basic flow of the node.

For example:

1. Reads data from all sensors intergrated into the Blue Robotics navigator flight controller pi hat
2. Performs any basic unit conversion and processing needed
3. Publishes to the following topics (defaults, can be configured using node parameters)
    - IMU raw acceleration data: `/imu/data_raw`, type: `sensor_msgs/msg/imu`
    - IMU raw magnetometer data: `/imu/mag`, type: `sensor_msgs/msg/magnetic_field`
    - Temperature: `/pi_temperature`, type: `sensor_msgs/msg/temperature`
    - Barometric Pressure: `/barometer/pressure"` , type: `sensor_msgs/msg/fluid_pressure`

4. A frame ID is added to the IMU topics(default: `imu`, configurable by parameter)
5. IMU sensors are polled with a configurable delay(default 15ms). All other sensors are polled at configurable delay(default 1000ms). 


## Important parts

* Uses the Blue Robotics library to interface with the flight controller hardware. The CMAKE file for this node was modifed following the example CMAKE file in the library repo. This allows `colcon build` to download library from GitHub and build it.





## What has been done

* Flight controller self-check
* IMU sensors
* Temperature sensor
* Barometric pressure sensor




## To do

* Read ADC channels
* Leak sensor reading
* Better IMU data conversion/processing
* LED output control







## Known issues / gotchas

* Magnetic field reading are returned from the library as microteslas, divided by 1e6 to convert to teslas as required for the `sensor_msgs/msg/magnetic_field` message type.
* Angular velocities seems to be to large by a factor of about 4 when visualizing in Foxglove the raw data filtered using imu tools, dividing them by 4 for now. Need to dig into this more as this fix may not be correct.
* Mapping of the axis from the sensor to the output topics may not be correct right now and also will change depending on the orientation of the pi in the glider, may need to change this mapping down the road.


## Testing

How have we tested this?

* [x] Runs locally
* [x] Runs on PI
* [ ] Integration tested
* [ ] Tested in water
* [ ] Tested on the glider






## Useful references

Anything that helped while working on this node:

* https://bluerobotics.com/store/comm-control-power/control/navigator/
* https://github.com/bluerobotics/navigator-lib






## Scratchpad / random stuff
