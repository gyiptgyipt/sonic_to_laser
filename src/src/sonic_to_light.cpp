#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/float32.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"


class S2L_converter : public rclcpp::Node {
public:
    S2L_converter(): Node("S2L_converter") {
              
        laser_publisher_= this->create_publisher<sensor_msgs::msg::LaserScan>("/laser", 1000);
        
        sonic_subscriber = this->create_subscription<std_msgs::msg::Float32>(
            "/sonar", 10, std::bind(&S2L_converter::sensor_callback, this, std::placeholders::_1));

        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);



    }

private:


    void sensor_callback( const std::shared_ptr<std_msgs::msg::Float32> msg) {
       

        // Sensor part

        // rclcpp::Time now = this->get_clock()->now();
        // std_msgs::msg::Float32 sonic;
        sensor_msgs::msg::LaserScan laser;
        


        rclcpp::Time end = this->get_clock()->now();
        // auto dt = (end-now).seconds();

        laser.header.stamp = end;
        laser.header.frame_id = "laser_frame";

        laser.angle_min = 0*M_PI/180;
        laser.angle_max = 90*M_PI/180;
        laser.angle_increment = M_PI/180;
        laser.time_increment = 0.001;
        laser.scan_time = 0.01;
        laser.range_min = 0.01;
        laser.range_max = 4;

        _Float32 cm_to_m = (_Float32)msg->data/100;
        if (cm_to_m > laser.range_max){
            cm_to_m = 4;}
        else if (cm_to_m == 0){
            cm_to_m = laser.range_min;}
        
        // double distance = ("%.1f",round(cm_to_m));
        _Float32 distance = std::ceil(cm_to_m *100.0)/100.0;

        
        uint32_t ranges_size = std::ceil((laser.angle_max - laser.angle_min) / laser.angle_increment);
        laser.ranges.assign(ranges_size,distance);
        laser.ranges.push_back(distance);
        // laser.ranges.resize(distance);
        // laser.intensities.resize(distance);
        
        laser_publisher_->publish(laser);

        // Transform part

        geometry_msgs::msg::TransformStamped transform_stamped;

        transform_stamped.header.stamp = this->get_clock()->now();
        transform_stamped.header.frame_id ="imu_frame_bridge";  // Parent frame
        transform_stamped.child_frame_id = "laser_frame";   // Child frame
        transform_stamped.transform.translation.x = 0;  
        transform_stamped.transform.translation.y = 0.0;
        transform_stamped.transform.translation.z = 0.27; 

        tf2::Quaternion q;
        q.setRPY(0, 0, 0);
        transform_stamped.transform.rotation.x = q.x();
        transform_stamped.transform.rotation.y = q.y();
        transform_stamped.transform.rotation.z = q.z();
        transform_stamped.transform.rotation.w = q.w();
        // RCLCPP_INFO(this->get_logger(), "I heard: '%f'", msg->angular_velocity.y);

        
        // transform_stamped.transform.rotation = tf2::toMsg(tf_orientation);

        // Broadcast the transformation
        tf_broadcaster_->sendTransform(transform_stamped);

    
        

    }


    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr laser_publisher_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sonic_subscriber;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<S2L_converter>());
    rclcpp::shutdown();
    return 0;
}