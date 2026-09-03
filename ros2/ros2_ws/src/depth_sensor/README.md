# `Depth Sensor`

- **Package:** `depth_sensor`
- **Executable:** `depth_publisher`
- **Status:** `Working`
- **People:** `Igor`




## How to run it

The simplest way to get this node running:

**Prereqs**
* Depth sensor connected to I2C6 port on pi hat
* `colcon build`
* `source install/setup.bash`

**Command**
```bash
ros2 run depth_sensor depth_publisher
```
*Integrated into launch*



## How it works

1. Reads temperature and pressure data from ms5837 sensor driver
2. Publishes temperature to *sensors/temperature* topic
3. Publishes pressure to *sensors/pressure* topic





## Data Visualization

Data visualization is done by `data.py`, a python script that uses matplotlib library to visualize collected data stored by rosbags **(from running using launch)**

**Run with**
```
python3 data.py
```

**Outputs:**
* temperature_plot.png
* pressure_plot.png





## Important parts

* `depth_sensor/depth_publisher.py` : main file that reads and publishes data
* `ms5837.py` : sensor driver
* `data.py` : data visualization script
* `/depth_temperature_bag` : where rosbag stores temperature data (db3)
* `/depth_pressure_bag` : where rosbag stores pressure data (db3)




## To do

* clean up code
* pipeline data to a dashboard to save data








## Known issues / gotchas
none






## Testing

How have we tested this?

* [x] Runs locally
* [x] Runs on PI with sensor
* [ ] Tested with integration
* [ ] Tested in water
* [ ] Tested on the glider





## Useful references

* https://github.com/bluerobotics/ms5837-python 
* https://bluerobotics.com/store/sensors-cameras/sensors/bar-depth-pressure-sensor/






## Scratchpad / random stuff
