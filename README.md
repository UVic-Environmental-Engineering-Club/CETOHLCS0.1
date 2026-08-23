# CETOHLCS0.1

## Running the ROS2 Docker Container

### Prereqs
* Docker installed and running
* Cloned repository

### Steps
1. **Navigate to the Docker directory in cloned repo:**
     ```bash
     cd ros2-docker
     ```

2. **Start the container:**
   * Standard (with hardware devices):
     ```bash
     docker compose up -d
     ```
   * development without connected devices):
     ```bash
     docker compose -f docker-compose-nopi.yml up -d
3. **Access the interactive shell:**
     ```bash
     docker exec -it cetolhlcs-ros2-node /bin/bash
     ```
4. **Stop the container:**
   * Standard:
     ```bash
     docker compose down
     ```
   * No-Pi:
     ```bash
     docker compose -f docker-compose-nopi.yml down
     ```


## Flight Controller Sensors Dev Setup

### Prereqs
- Docker container running
- Terminal connected to the container interactive shell

### Steps

Steps 2-4 only need to be completed once after a new setup of the container

1. Navigate to the ros2 workspace

```
cd /ros2/ros2_ws/
```

2. Install dependencies using rosdep

```
rosdep install --from-paths src --ignore-src -r -y
```

3. Build ros2 packages using colcon and source to current terminal

```
colcon build
```

4. Install foxglove bridge
```
sudo apt update
sudo apt install ros-$ROS_DISTRO-foxglove-bridge
```

5. Source workspace
```
source install/setup.bash
```

6. Run flight controller sensors ros2 node
```
ros2 run flight_controller_sensors flight_controller_sensors_publisher
```
If hardware is functioning correctly these messages should be shown:
```
[INFO] [1787517714.892005557] [flight_controller_sensors_publisher]: Initiating navigator module.
[INFO] [1787517714.892559825] [flight_controller_sensors_publisher]: Navigator self test passed.
```

7. Start the Imu tools madgwick filter node
```
ros2 launch imu_filter_madgwick imu_filter.launch.py
```

8. Start the foxglove bridge
```
ros2 launch foxglove_bridge foxglove_bridge_launch.xml
```

9. View data in foxglove 

Navigate to `app.foxglove.dev` (creating an account if required). Select the `Create Connection` and option and choose `Foxglove WebSocket` the fill in the adderess in this format: `ws://[ip of pi]:8765`
