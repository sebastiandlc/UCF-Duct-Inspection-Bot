#!/usr/bin/env python

import rospy
from std_msgs.msg import Float64MultiArray
import tf
from nav_msgs.msg import Odometry
import math
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3

odom_pub = rospy.Publisher("/odom", Odometry, queue_size=50)

def callback(position):
    vel_l = position.data[0] / 10000
    vel_r = position.data[1] / 10000
      
    odom_broadcaster = tf.TransformBroadcaster()

    #br = tf.TransformBroadcaster()

    current_time = rospy.Time.now()
    last_time = rospy.Time.now()
    r = rospy.Rate(0.1)

    current_time = rospy.Time.now()

    dt = (current_time - last_time).secs

    v_right = convert_to_vel(vel_r) * 0.0254
    v_left = convert_to_vel(vel_l) * 0.0254

    #velocity in robot frame
    v_rx = v_right                     
    v_ry = 0;
    if((v_right >= 0 and v_left >= 0) or (v_right <= 0 and v_left <= 0)):
      omega_r = v_right / 0.12845
    else:
      omega_r = (v_right - v_left) / 0.12845

    #print("omega_r: " + str(omega_r))     

    # velocity in odom frame
    v_wx = v_rx * math.cos(omega_r) - v_ry * math.sin(omega_r)
    v_wy = v_rx * math.sin(omega_r) + v_ry * math.cos(omega_r)
    thetadot = omega_r

    #pose
    x_kp1 = (v_rx * dt) + v_wx * dt
    y_kp1 = (v_ry * dt) + v_wy * dt
    theta_kp1 = omega_r + thetadot * dt

    odom_quat = tf.transformations.quaternion_from_euler(0, 0, theta_kp1)

    #br.sendTransform((x_kp1,y_kp1,0), odom_quat, current_time, "base_link","world")

    odom_broadcaster.sendTransform(
        (x_kp1,y_kp1, 0.),
        odom_quat,
        current_time,
        "base_link",
        "/odom"
    )
 
    odom = Odometry()
    odom.header.stamp = current_time
    odom.header.frame_id = "/odom"

    odom.pose.pose = Pose(Point(x_kp1, y_kp1, 0.), Quaternion(*odom_quat))
    odom.child_frame_id = "base_link"
    odom.twist.twist = Twist(Vector3(v_rx,0,0), Vector3(0,0,theta_kp1))
    last_time = current_time
    odom_pub.publish(odom)

def convert_to_vel(speed):
    degrees = 360/129.5
    rpm = speed * degrees * 60/360
    # r = 0.035
    linear_vel = (0.0254 * 2 * math.pi * rpm)/60 # m/s
    #print("vel" + str(linear_vel))
    return linear_vel

def odom_publish():
    rospy.init_node('odom')
    rospy.Subscriber("/pose_tic", Float64MultiArray, callback)
    rospy.spin()
    


odom_publish()


