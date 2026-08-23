#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

#include "sensor_msgs/msg/imu.hpp"
#include "sensor_msgs/msg/magnetic_field.hpp"
#include "sensor_msgs/msg/temperature.hpp"

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
            _imu_poll_rate = this->declare_parameter<int>("imu_poll_rate", 15);
            _adc_poll_rate = this->declare_parameter<int>("adc_poll_rate", 100);
            _sensor_poll_rate = this->declare_parameter<int>("sensor_poll_rate", 1000);

            _topicImu = this->declare_parameter<std::string>("topicImu", "/imu/data_raw");
            _topicMag = this->declare_parameter<std::string>("topicMag", "/imu/mag");
            _topicTemp = this->declare_parameter<std::string>("topicTemp", "/pi_temperature");
            _publisherIMU = this->create_publisher<sensor_msgs::msg::Imu>(_topicImu, 10);
            _publisherMag = this->create_publisher<sensor_msgs::msg::MagneticField>(_topicMag, 10);
            _publisherTemp = this->create_publisher<sensor_msgs::msg::Temperature>(_topicTemp, 10);

            _imu_timer = this->create_wall_timer(
                std::chrono::milliseconds(_imu_poll_rate),
                std::bind(&FlightControllerSensorsPublisher::publish_imu_data, this));

            _adc_timer = this->create_wall_timer(
                std::chrono::milliseconds(_adc_poll_rate),
                std::bind(&FlightControllerSensorsPublisher::publish_adc_data, this));

            _sensor_timer = this->create_wall_timer(
                std::chrono::milliseconds(_sensor_poll_rate),
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

        void publish_imu_data()
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
        
        void publish_adc_data()
        {
            // Placeholder for ADC data publishing logic
        }

        void publish_sensor_data()
        {
            float temperature = read_temp();

            auto temp_msg = sensor_msgs::msg::Temperature();
            temp_msg.header.stamp = this->now();
            temp_msg.temperature = temperature;

            _publisherTemp->publish(temp_msg);
        }

        rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr _publisherIMU;
        rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr _publisherMag;
        rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr _publisherTemp;
        rclcpp::TimerBase::SharedPtr _imu_timer;
        rclcpp::TimerBase::SharedPtr _adc_timer;
        rclcpp::TimerBase::SharedPtr _sensor_timer;

        int _imu_poll_rate;
        int _adc_poll_rate;
        int _sensor_poll_rate;

        std::string _frame_id;

        std::string _topicImu;
        std::string _topicMag;
        std::string _topicTemp;
};

int main(int argc, char * argv[])
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<FlightControllerSensorsPublisher>();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

