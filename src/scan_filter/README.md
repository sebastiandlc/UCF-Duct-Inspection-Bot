# Scan Filter for Lidar Sensor

The lidar sensor onboard the robot (RPLiDAR A1) has a 360 degree view by default. In our case, we needed to limit this viewing angle through the use of scan_filter.py. This script reads the values from the original laserscan and then reconstructs a new laser scan while ignoring any datapoint that falls outside the range of (-pi/4, pi/4) and then publishes this new scan under a new name.
