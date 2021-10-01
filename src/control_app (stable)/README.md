# Control Application

Browser based application used to control the robot.

**This version requires direct ethernet connection in order to function properly.**

### About

This application allows for the control of the system through the use various ROS packages. Basic ROS communication involves the use of [Topics](http://wiki.ros.org/Topics). To communicate, nodes either subscribe (receive) or publish (send) to other nodes. In this case, the control app serves as a publisher for the `/cmd_vel` Topic. Twist messages (comprised of both a linear and angular velocity vector) are created using user input, and published to `/cmd_vel`, of which a separate node on the robot is a subscriber to.
