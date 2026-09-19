from launch import LaunchDescription
from datetime import datetime
from launch.actions import ExecuteProcess, DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # Launch flags
    depth_sensor = LaunchConfiguration('depth_sensor')
    experimental = LaunchConfiguration('experimental')

    # Generate the dynamic path with timestamp (e.g., /ros2/ros2_ws/recordings/rosbag_2026_09_19-14_30_00)
    timestamp = datetime.now().strftime('%Y_%m_%d-%H_%M_%S')
    bag_output_path = f"/ros2/ros2_ws/recordings/CETOHLCS_rosbag_{timestamp}"

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
        # Recording
        # ---------------------------------------------------------------------
        

        # Record temperature
        ExecuteProcess(
            cmd=[
                'ros2', 'bag', 'record',
                '-s', 'mcap',
                '--all',
                '-o', bag_output_path
            ],
            output='screen'
        ),

        # ---------------------------------------------------------------------
        # Depth sensor
        # ---------------------------------------------------------------------

        # Depth sensor node
        Node(
            package='depth_sensor',
            executable='depth_publisher',
            name='depth_sensor_node',
            output='screen',
            emulate_tty=True,
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
