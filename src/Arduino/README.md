**Arduino code not part of a ROS package will go here under the sketchbook.**

This system is powered by stepper motors, and in turn are controlled by Pololu Tic T500 Motor Controllers onboard. The Tic supports the I2C serial communication protocol, through which we wired to two separate Arduinos: one for the track system and one for the scissor lift.

Pololu also provides Arduino libraries to interface with their boards which we used to get and set velocity values through ROS Twist messages. Twist messages are comprised of both a linear velocity vector, and an angular velocity vector. Values for these vectors are retrieved from the movement controls found on the control app. Once these vectors are generated, the newly created Twist message is published to the topic /cmd_vel. Since the Arduinos were setup through the rosserial package to subscribe to the /cmd_vel topic, they are able to send these controls to the Tic controllers and in turn, the stepper motors, allowing for the robot to move it's tracks and scissor lift.



I2CSpeedControl.ino controls the speed of the stepper motors using I2C commands.

TwistTeleop.ino and ScissorLift.ino control the motors for the tracks and scissor lift respectively.
