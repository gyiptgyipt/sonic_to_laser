# SONIC TO LASER 

This is the tutorial for understanding the use of ultrasonic waves and laser.
We also observe their characteristics and properties.

### Benefits 

-use of slam 
-use of point cloud 
-understanding angle increment of those two sensors
-micro_ros_arduino
-using IMU as rotation using IMU_tools ( IMU drift over time) 

### Requirements

- [Gyro_micro_ros tutorial](https://github.com/Htet-Wai-Yan-HWY/gyro_micro_ros)
- slam-tool-box
- ```
  sudo apt-get install ros-<your-ROS-DISTRO>-slam-tool-box
  ```

## Tutorial

- Go to your workspace
- Download the repository
- ```
  git clone https://github.com/gyiptgyipt/sonic_to_laser.git
  ```
- Build the project
- ```
  colcon build --symlink-install
  ```
- ```
  source install/setup.bash
  ```
  
- Upload the [micro_ros program](https://github.com/gyiptgyipt/sonic_to_laser/blob/main/src/ultrasonic_to_lidar/src/src/slam_micro_ros.ino) to your microchip.(don't forget to change the SSID,PASSOWRD AND TARGET IP)
### warning 
    
make sure that installing micro_ros_arduino library.
    
library link : [micro_ros_arduino](https://github.com/micro-ROS/micro_ros_arduino/releases)

If it not installed, [micro_ros.org](https://micro.ros.org/docs/tutorials/core/first_application_linux/) and follow the tutorial.

-In that link you can use to upload firmware with that packages but there is a little probem with my machine, so I used above methods. And build and source declear that project in your ros2 workspace.

-I just use the micro_ros_agent from that tutorial.(Because we uploaded our firmware with arduino IDE)


In this project, I use wifi udp4 to get the data from micro_controller.
### warning : Don't use 24,25 pins for joystick module in esp32 (That used by wifi).


If it already installed,  run micro ros agent 
    
```
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888
```

And than you will see the node and topic in host computer(that use targeted IP address)

- And Run
- ```
  ros2 run sonic_to_laser sonic_to_laser
  ```
- after that /laser topic formed from /sonic (Changed sonic data to laser data )

## SlAM 

```
ros2 launch sonic2laser_map online_async_launch_mapping.launch.py
```
-Open rviz2.

```
rviz2
```

-Set the global frame to ```map``` and

-Start vizualize your map.

-<If you want to save map, use map saver and map server packages>

-[YOUTUBE_VIDEO_LINK](https://www.youtube.com/shorts/Iqim6FD3cGk)


### Free to ask me anytime if something was wrong, I love to help you guys <3
