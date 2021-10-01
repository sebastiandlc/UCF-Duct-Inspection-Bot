# Odometry

The mapping algorithm used for our robot is in the ROS package gmapping, which contains a ROS wrapper for the open source mapping algorithm OpenSLAM. This is a 2D SLAM (Simultaneous Localization and Mapping) algorithm that uses laser scan input from a Lidar and odometry data from our robot. The odometry data provides the algorithm with an estimate of the current pose of the robot in the environment, allowing us to create a 2D floorplan of the ducts.

For gmapping to work, an odometry topic, and a laser scan topic from the lidar are required. These topics then need to be "transformed," meaning they need to be relative to the pose of the robot. This allows gmapping to see how the scans are reading in comparison to the position of the robot.
