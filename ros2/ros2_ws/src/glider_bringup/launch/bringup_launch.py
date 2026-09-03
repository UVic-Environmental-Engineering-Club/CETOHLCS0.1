from launch import LaunchDescription
from launch.actions import ExecuteProcess, DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # Launch flags
    depth_sensor = LaunchConfiguration('depth_sensor')
    experimental = LaunchConfiguration('experimental')

    return LaunchDescription([

        # ---------------------------------------------------------------------
        # Launch arguments
        # ---------------------------------------------------------------------

        DeclareLaunchArgument(
            'depth_sensor',
            default_value='true',
            description='Launch the depth sensor and its data recording'
        ),

        DeclareLaunchArgument(
            'experimental',
            default_value='false',
            description='Launch experimental nodes'
        ),


        # ---------------------------------------------------------------------
        # Depth sensor
        # ---------------------------------------------------------------------

        # Delete previous recordings
        ExecuteProcess(
            cmd=[
                'rm', '-rf',
                '/ros2/ros2_ws/src/depth_sensor/depth_pressure_bag',
                '/ros2/ros2_ws/src/depth_sensor/depth_temperature_bag'
            ],
            output='screen',
            condition=IfCondition(depth_sensor)
        ),

        # Depth sensor node
        Node(
            package='depth_sensor',
            executable='depth_publisher',
            name='depth_sensor_node',
            output='screen',
            emulate_tty=True,
            condition=IfCondition(depth_sensor)
        ),

        # Record temperature
        ExecuteProcess(
            cmd=[
                'ros2', 'bag', 'record',
                '-o',
                '/ros2/ros2_ws/src/depth_sensor/depth_temperature_bag',
                '/sensors/temperature'
            ],
            output='screen',
            condition=IfCondition(depth_sensor)
        ),

        # Record pressure
        ExecuteProcess(
            cmd=[
                'ros2', 'bag', 'record',
                '-o',
                '/ros2/ros2_ws/src/depth_sensor/depth_pressure_bag',
                '/sensors/pressure'
            ],
            output='screen',
            condition=IfCondition(depth_sensor)
        ),


        # ---------------------------------------------------------------------
        # Other sections
        # ---------------------------------------------------------------------

        # Example:
      
        # Node(
        #     package='example_package',
        #     executable='example_executable',
        #     name='example_node',
        #     output='screen',
        #     condition=IfCondition(LaunchConfiguration('example'))
        # ),


        # ---------------------------------------------------------------------
        # Experimental - DEFAULT OFF
        # ---------------------------------------------------------------------
 
        # Node(
        #     package='experimental_package',
        #     executable='experimental_node',
        #     output='screen',
        #     condition=IfCondition(experimental)
        # ),

    ])