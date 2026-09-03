# `glider_bringup`

- **Package:** `glider_bringup`
- **launch file:** `bringup_launch.py`
- **Status:** `Working`
- **People:** `Igor`






## How to run it

**prereqs**
* `colcon build`
* `source install/setup.bash`

* *Depth Sensor*
  * Depth sensor connected to I2C6 port on pi hat

**Command**

```bash
ros2 launch glider_bringup bringup_launch.py
```

**Flags**
All nodes included with launch are default active (except for those under experimental)

nodes can be included or not included using flags: 
```
ros2 launch glider_bringup bringup_launch.py depth_sensor:=false experimental:=true
```

## Nodes

#### Depth Sensor
* Deletes current saved data for pressure and temperature
* Runs depth sensor node with rosbags
  * temperature data is stored in depth_sensor/depth_temperature_bag
  * pressure data is stored in depth_sensor/depth_pressure_bag





## Important parts

* `/launch/bringup_launch.py` : main file for all launch configurations
 










## To do

* Integrate Other devices








## Known issues / gotchas

* Starting the depth sensor through bringup currently deletes the previous pressure and temperature ROS bags before recording new data.

* ROS 2 launch starts processes asynchronously. Do not assume that one node has completely initialized before another one starts.

* Experimental nodes should remain disabled by default until they are ready for integration.






## Useful references
