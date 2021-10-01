//rosrun rosserial_python serial_node.py /dev/ttyUSB0
//rosrun teleop_twist_keyboard teleop_twist_keyboard.py


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif
#include <stdlib.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
//#include <Servo.h>
#include <Tic.h>
ros::NodeHandle nh;
geometry_msgs::Twist msg;


float move1;
float move2;

int maxPos = 26000;
int minPos = 0;

TicI2C tic1(18);
TicI2C tic2(19);

int32_t currentPositionR;
int32_t currentPositionL;

void callback(const geometry_msgs::Twist& cmd_vel)
{
  move1 = cmd_vel.linear.x;
  move2 = cmd_vel.angular.z;

  if (move1 > 0 && move2 > 0 )
  {
    up();
  }
  else if (move1 > 0 && move2 < 0 )
  {
    down();
  }
  else if (move1 < 0 && move2 > 0)
  {
    energizeMotors();
  }
  else if (move1 < 0 && move2 < 0)
  {
    deenergizeMotors();
  }
}

ros::Subscriber <geometry_msgs::Twist> sub("/cmd_vel", callback);

void setup()
{
  Serial.begin(57600);
  Wire.begin();

  delay(20);

  nh.initNode();
  nh.subscribe(sub);

  tic1.exitSafeStart();
  tic2.exitSafeStart();

  tic1.energize();
  tic2.energize();
}

void loop()
{
  nh.spinOnce();

  currentPositionR = tic1.getCurrentPosition();
  currentPositionL = tic2.getCurrentPosition();

  delay(1);
}

void up()
{
    if (currentPositionL < maxPos)
    {
        if (currentPositionL > (maxPos - 1000) && currentPositionL < maxPos)
        {
          tic1.setTargetPosition(maxPos);
          tic2.setTargetPosition(maxPos);
        }
        else
        {
          tic1.setTargetPosition(currentPositionR + 1000);
          tic2.setTargetPosition(currentPositionL + 1000);
        }
    }
}

void down()
{
    if (currentPositionL > minPos)
    {
        if (currentPositionL < 1000 && currentPositionL > minPos)
        {
          tic1.setTargetPosition(0);
          tic2.setTargetPosition(0);
        }
        else
        {
          tic1.setTargetPosition(currentPositionR - 1000);
          tic2.setTargetPosition(currentPositionL - 1000);
        }
    }
}

void energizeMotors()
{
    tic1.exitSafeStart();
    tic2.exitSafeStart();

    tic1.energize();
    tic2.energize();
}

void deenergizeMotors()
{
    tic1.deenergize();
    tic2.deenergize();
}

void shutdwn()
{
    tic1.setTargetPosition(0);
    tic2.setTargetPosition(0);

    tic1.deenergize();
    tic2.deenergize();
}
