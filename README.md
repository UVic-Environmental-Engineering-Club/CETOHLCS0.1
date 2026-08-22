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


