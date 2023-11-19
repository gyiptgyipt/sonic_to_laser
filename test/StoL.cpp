#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/int32.hpp"



#define PI 3.14159265
#define DEG_TO_RAD(x) (x * PI / 180.0)
#define MAX_RANGE 3.0 // meters
#define MIN_ANGLE -144 // degrees
#define MAX_ANGLE 144 // degrees
#define DEG_INCREMENT 2 // degrees
#define NUM_READINGS ((MAX_ANGLE-MIN_ANGLE) / DEG_INCREMENT + 1)

#define NUMBER_OF_SONAR_SENSORS 8
#define magnitude(x,y) sqrt((x)*(x) + (y)*(y))


double sonar_frequency = 25;

double ranges[NUM_READINGS];
double intensities[NUM_READINGS];


class rev_tracker : public rclcpp::Node {
public:
    rev_tracker(): Node("rev_tracker") {
              
        laser_publisher_= this->create_publisher<sensor_msgs::msg::LaserScan>("/laser", 10);
        
        sonic_subscriber = this->create_subscription<std_msgs::msg::Int32>(
            "/imu/quaternion", 10, std::bind(&rev_tracker::sensor_callback, this, std::placeholders::_1));



    }

private:


    void sensor_callback( const std::shared_ptr<std_msgs::msg::Int32> msg) {
       
        rclcpp::Time now = this->get_clock()->now();

        
    
        std_msgs::msg::Int32 sonic;
        sensor_msgs::msg::LaserScan laser;
        
        double cm_to_m = (double)sonic.data/100;
        if (cm_to_m >4){
            cm_to_m = 4;}
        else if (cm_to_m == 0){
            cm_to_m = 0.01;}
        
        double distance = round(cm_to_m);

        rclcpp::Time end = this->get_clock()->now();
        auto dt = (end-now).seconds();

        laser.header.stamp = end;
        laser.header.frame_id = 'sonar';
        laser.angle_min = 0*M_PI/180;
        laser.angle_max = 165*M_PI/180;
        laser.angle_increment = M_PI/180;
        laser.time_increment = 0.001;
        laser.scan_time = 0.001;
        laser.range_min = 0.01;
        laser.range_max = 4;

        // laser.ranges = distance;


        laser_publisher_->publish(laser);

    
        
        // transform_stamped.header.stamp = this->get_clock()->now();




        // RCLCPP_INFO(this->get_logger(), "I heard: X : '%f' Y : '%f' Z : '%f", roll,pitch,yaw);


        // auto lin_vel_x = msg->angular_velocity.x * wheel_radius ;
        // auto lin_vel_y = msg->angular_velocity.y * wheel_radius ;

        // // auto dis_x = lin_vel_x * time_now;
        // twist.linear.x = lin_vel_x;
        // twist.linear.y = lin_vel_y;
        // //twist.angular.z = msg->angular_velocity.z;

        // publisher_->publish(twist);


    }


    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr laser_publisher_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sonic_subscriber;

};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<rev_tracker>());
    rclcpp::shutdown();
    return 0;
}