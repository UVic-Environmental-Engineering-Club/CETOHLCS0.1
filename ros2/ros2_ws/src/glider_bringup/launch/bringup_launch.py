import os
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from datetime import datetime
from launch.actions import ExecuteProcess, DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource, AnyLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():

    # Launch flags
    depth_sensor = LaunchConfiguration('depth_sensor')
    flight_controller_sensors = LaunchConfiguration('flight_controller_sensors')
    experimental = LaunchConfiguration('experimental')

    # Generate the dynamic path with timestamp (e.g., /ros2/ros2_ws/recordings/rosbag_2026_09_19-14_30_00)
    timestamp = datetime.now().strftime('%Y_%m_%d-%H_%M_%S')
    bag_output_path = f"/ros2/ros2_ws/recordings/CETOHLCS_rosbag_{timestamp}"

    # Get the path to the imu_filter_madgwick package
    imu_filter_dir = get_package_share_directory('imu_filter_madgwick')
    imu_filter_launch_path = os.path.join(imu_filter_dir, 'launch', 'imu_filter.launch.py')

    foxglove_bridge_dir = get_package_share_directory('foxglove_bridge')
    foxglove_launch_path = os.path.join(foxglove_bridge_dir, 'launch', 'foxglove_bridge_launch.xml')

    return LaunchDescription([

        # ---------------------------------------------------------------------
        # Launch arguments
        # ---------------------------------------------------------------------

        DeclareLaunchArgument(
            'depth_sensor',
            default_value='true',
            description='Launch the depth sensor'
        ),

        DeclareLaunchArgument(
            'flight_controller_sensors',
            default_value='true',
            description='Launch the flight controller sensors'
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
        # Flight Controller Sensors
        # ---------------------------------------------------------------------

        # Depth sensor node
        Node(
            package='flight_controller_sensors',
            executable='flight_controller_sensors_publisher',
            name='flight_controller_sensors_node',
            output='screen',
            emulate_tty=True,
            condition=IfCondition(flight_controller_sensors)
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(imu_filter_launch_path),
            condition=IfCondition(flight_controller_sensors)
        ),

        # ---------------------------------------------------------------------
        # Foxglove Bridge
        # ---------------------------------------------------------------------

        # Include the Foxglove Bridge XML launch file
        IncludeLaunchDescription(
            AnyLaunchDescriptionSource(foxglove_launch_path)
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
