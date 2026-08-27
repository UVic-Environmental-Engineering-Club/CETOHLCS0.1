from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='depth_sensor',
            executable='depth_publisher',
            name='depth_sensor_node',
            output='screen',
            emulate_tty=True
        ),

        ExecuteProcess(
            cmd=['ros2', 'bag', 'record', '-o', '/ros2/ros2_ws/src/depth_sensor/depth_temperature_bag', '/sensors/temperature'],
            output='screen'
        ),
        
        ExecuteProcess(
            cmd=['ros2', 'bag', 'record', '-o', '/ros2/ros2_ws/src/depth_sensor/depth_pressure_bag', '/sensors/pressure'],
            output='screen'
        )
    ])