#!/bin/bash

#rosbridge
gnome-terminal -- bash -c "roslaunch robot_gui_bridge websocket.launch"

sleep 5s

#launch lidar
gnome-terminal -- bash -c "roslaunch rplidar_ros rplidar.launch"

#launch lidar laser filter
gnome-terminal -- bash -c "rosrun scan_filter scan_filter.py"

#rosserial for both arduinos
gnome-terminal -- bash -c "roslaunch rosserial_python rosserial.launch"

#odometry script
gnome-terminal -- bash -c "rosrun odom_publisher odom.py"

#mapping script
gnome-terminal -- bash -c "roslaunch gmapping gmapping.launch"

echo terminals launched

: $SHELL
