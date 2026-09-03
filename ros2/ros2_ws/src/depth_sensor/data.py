"""
data.py
creates two plots for temperature and pressure readings

To do: 
  - Remove duplicate code
"""

from pathlib import Path
import matplotlib.pyplot as plt
from rosbags.rosbag2 import Reader
from rosbags.typesys import Stores, get_typestore

typestore = get_typestore(Stores.ROS2_HUMBLE)

#pressure plotting ---------------------------------------------------------------------------
bag_path = Path("./depth_pressure_bag")

timestamps = []
pressures = []

with Reader(bag_path) as reader:
    connections = [x for x in reader.connections if x.topic == "/sensors/pressure"]

    for connection, timestamp, rawdata in reader.messages(connections=connections):
        msg = typestore.deserialize_cdr(rawdata, connection.msgtype)
        timestamps.append(timestamp / 1e9)
        pressures.append(msg.fluid_pressure)

start_time = timestamps[0]
relative_time = [t - start_time for t in timestamps]

plt.figure(figsize=(10, 5))
plt.plot(relative_time, pressures, marker="o", linestyle="-", color="tab:blue", label="Pressure")
plt.title("Underwater Glider - Fluid Pressure over Time")
plt.xlabel("Elapsed Time (seconds)")
plt.ylabel("Pressure (Pa)")
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig("pressure_plot.png", dpi=300)
print("Pressure plot saved to pressure_plot.png") 


#temperature plotting ---------------------------------------------------------------------------

bag_path = Path("./depth_temperature_bag")

timestamps = []
temperatures = []

with Reader(bag_path) as reader:
    connections = [x for x in reader.connections if x.topic == "/sensors/temperature"]

    for connection, timestamp, rawdata in reader.messages(connections=connections):
        msg = typestore.deserialize_cdr(rawdata, connection.msgtype)
        timestamps.append(timestamp / 1e9)
        temperatures.append(msg.temperature)

start_time = timestamps[0]
relative_time = [t - start_time for t in timestamps]

plt.figure(figsize=(10, 5))
plt.plot(relative_time, temperatures, marker="o", linestyle="-", color="tab:blue", label="Temperature")
plt.title("Underwater Glider - Temperature over Time")
plt.xlabel("Elapsed Time (seconds)")
plt.ylabel("Temperature (C)")
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig("temperature_plot.png", dpi=300)
print("Temperature plot saved to temperature_plot.png") 