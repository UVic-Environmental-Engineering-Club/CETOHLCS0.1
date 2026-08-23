#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

#include "sensor_msgs/msg/imu.hpp"
#include "sensor_msgs/msg/magnetic_field.hpp"

#include "bindings.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


class FlightControllerSensorsPublisher : public rclcpp::Node
{
    public:
        FlightControllerSensorsPublisher() : Node("flight_controller_sensors_publisher")
        {

            _frame_id = this->declare_parameter<std::string>("frame_id", "imu");
            _poll_rate = this->declare_parameter<int>("poll_rate", 15);
            _topicImu = this->declare_parameter<std::string>("topicImu", "/imu/data_raw");
            _topicMag = this->declare_parameter<std::string>("topicMag", "/imu/mag");

            _publisherIMU = this->create_publisher<sensor_msgs::msg::Imu>(_topicImu, 10);
            _publisherMag = this->create_publisher<sensor_msgs::msg::MagneticField>(_topicMag, 10);

            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(_poll_rate),
                std::bind(&FlightControllerSensorsPublisher::publish_sensor_data, this));

            RCLCPP_INFO(this->get_logger(), "Initiating navigator module.\n");
            init();

            if(!self_test()){
                RCLCPP_ERROR(this->get_logger(), "Navigator self test failed. Exiting.\n");
                rclcpp::shutdown();
            }

            RCLCPP_INFO(this->get_logger(), "Navigator self test passed.\n");

        }
    
    private:

        void publish_sensor_data()
        {
            AxisData mag = read_mag();
            AxisData accel = read_accel();
            AxisData gyro = read_gyro();

            auto mag_msg = sensor_msgs::msg::MagneticField();
            mag_msg.header.frame_id = _frame_id;
            mag_msg.header.stamp = this->now();

            mag_msg.magnetic_field.x = mag.x/1000000.0; // Convert from microtesla to tesla
            mag_msg.magnetic_field.y = mag.y/1000000.0; 
            mag_msg.magnetic_field.z = mag.z/1000000.0; 

            _publisherMag->publish(mag_msg);

            auto imu_msg = sensor_msgs::msg::Imu();
            imu_msg.header.frame_id = _frame_id;
            imu_msg.header.stamp = mag_msg.header.stamp;

            imu_msg.linear_acceleration.x = accel.y;
            imu_msg.linear_acceleration.y = accel.x;
            imu_msg.linear_acceleration.z = accel.z;

            imu_msg.angular_velocity.x = gyro.y/4;
            imu_msg.angular_velocity.y = gyro.x/4;
            imu_msg.angular_velocity.z = gyro.z/4;

            _publisherIMU->publish(imu_msg);
        }    

        rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr _publisherIMU;
        rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr _publisherMag;
        rclcpp::TimerBase::SharedPtr timer_;

        int _poll_rate;

        std::string _frame_id;

        std::string _topicImu;
        std::string _topicMag;

};

int main(int argc, char * argv[])
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<FlightControllerSensorsPublisher>();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

