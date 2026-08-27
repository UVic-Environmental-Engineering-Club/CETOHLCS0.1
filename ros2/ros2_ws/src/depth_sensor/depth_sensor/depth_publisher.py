import rclpy
from . import ms5837
from rclpy.node import Node
from sensor_msgs.msg import FluidPressure, Temperature

class DepthPublisher(Node):
  
  def __init__(self):
    super().__init__('depth_publisher')
    
    self.sensor = ms5837.MS5837_30BA()
    if not self.sensor.init():
      self.get_logger().error('Failed to initialize sensor')

    self.pressure_pub = self.create_publisher(FluidPressure, 'sensors/pressure', 10)
    self.temp_pub = self.create_publisher(Temperature, 'sensors/temperature', 10)
    self.timer = self.create_timer(1.0, self.read_and_publish)

  def read_and_publish(self):
    if not self.sensor.read():
      self.get_logger().warn('Sensor read failed')
      return
    now = self.get_clock().now().to_msg()

    pressure_msg = FluidPressure()
    pressure_msg.header.stamp = now
    pressure_msg.fluid_pressure = self.sensor.pressure(ms5837.UNITS_Pa)
    self.pressure_pub.publish(pressure_msg)

    temp_msg = Temperature()
    temp_msg.header.stamp = now
    temp_msg.temperature = self.sensor.temperature()
    self.temp_pub.publish(temp_msg)

def main():
    rclpy.init()
    node = DepthPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
