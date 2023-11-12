#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>

#include "sensor_msgs/msg/imu.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"


class IMU2Twist : public rclcpp::Node {
public:
    IMU2Twist(): Node("IMU2Twist") {
              
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("imu_twist", 10);
        
        imu_subscriber_ = this->create_subscription<sensor_msgs::msg::Imu>(
            "/imu/data", 10, std::bind(&IMU2Twist::imuCallback, this, std::placeholders::_1));



    }

private:

    void imuCallback( const std::shared_ptr<sensor_msgs::msg::Imu> msg) {
       

        geometry_msgs::msg::TransformStamped transform_stamped;
        geometry_msgs::msg::Twist twist;
        
        transform_stamped.header.stamp = this->get_clock()->now();

        rclcpp::Time now = this->get_clock()->now();
        rclcpp::Time when = this->get_clock()->now() - rclcpp::Duration(5, 0);
        rclcpp::Time t_delta = now + rclcpp::Duration(1/10, 0);

        double time_now = now.seconds();

        auto wheel_radius = 0.0508;
        auto lin_vel_x = msg->angular_velocity.x * wheel_radius ;
        auto lin_vel_y = msg->angular_velocity.y * wheel_radius ;

        // auto dis_x = lin_vel_x * time_now;
        twist.linear.x = lin_vel_x;
        twist.linear.y = lin_vel_y;
        //twist.angular.z = msg->angular_velocity.z;

        publisher_->publish(twist);


    }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscriber_;

};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<IMU2Twist>());
    rclcpp::shutdown();
    return 0;
}