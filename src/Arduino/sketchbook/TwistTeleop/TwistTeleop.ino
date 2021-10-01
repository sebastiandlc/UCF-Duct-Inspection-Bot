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
#include <std_msgs/Float64MultiArray.h>
ros::NodeHandle nh;
geometry_msgs::Twist msg;

std_msgs::Float64MultiArray pos;

float move1;
float move2;


void callback(const geometry_msgs::Twist& cmd_vel)
{
  move1 = cmd_vel.linear.x;
  move2 = cmd_vel.angular.z;

  if (move1 > 0 && move2 == 0)
  {
    front(move1);
  }
  else if (move1 < 0 && move2 == 0)
  {
    back(move1);
  }
  else if (move1 < 0 && move2 > 0) {
    backLeft();
  }
  else if (move1 < 0 && move2 < 0) {
    backRight();
  }
  else if (move1 == 0 && move2 > 0) {
    spotLeft();
  }
  else if (move1 == 0 && move2 < 0) {
    spotRight();
  }
  else
  {
    die();
  }
}

ros::Subscriber <geometry_msgs::Twist> sub("/cmd_vel", callback);
ros::Publisher p("/pose_tic", &pos);

TicI2C tic1(14);
TicI2C tic2(15);
TicI2C tic3(16);
TicI2C tic4(17);

void setup()
{
  Serial.begin(57600);
  Wire.begin();

  delay(20);

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(p);

  pos.data = (float*)malloc(sizeof(float) * 2);
  pos.data_length = 2;

  tic1.exitSafeStart();
  tic2.exitSafeStart();
  tic3.exitSafeStart();
  tic4.exitSafeStart();
  tic1.energize();
  tic2.energize();
  tic3.energize();
  tic4.energize();
}

void loop()
{
  nh.spinOnce();
  unsigned long currentMillis = millis();

  if (currentMillis >= 100)
  {
    pos.data[0] = (float) tic1.getCurrentVelocity();
    pos.data[1] = (float) tic2.getCurrentVelocity();
    p.publish(&pos);
    currentMillis = 0;
  }

  delay(1);
}

void front(int mode)
{
    if (mode == 1)
    {
      tic1.setTargetVelocity(-20000000);
      tic2.setTargetVelocity(-20000000);
      tic3.setTargetVelocity(-20000000);
      tic4.setTargetVelocity(20000000);
    }
    else if (mode == 2)
    {
      tic3.setTargetVelocity(-20000000);
      tic4.setTargetVelocity(20000000);
    }
    else if (mode == 3)
    {
      tic1.setTargetVelocity(-20000000);
      tic2.setTargetVelocity(-20000000);
    }
    else if (mode == 4)
    {
      tic1.setTargetVelocity(-20000000);
      tic2.setTargetVelocity(-20000000);
      tic3.setTargetVelocity(-800000);
      tic4.setTargetVelocity(800000);
    }
    else if (mode == 5)
    {
      tic1.setTargetVelocity(-800000);
      tic2.setTargetVelocity(-800000);
      tic3.setTargetVelocity(-20000000);
      tic4.setTargetVelocity(20000000);
    }
}

void back(int mode)
{
    if (mode == -1)
    {
      tic1.setTargetVelocity(20000000);
      tic2.setTargetVelocity(20000000);
      tic3.setTargetVelocity(20000000);
      tic4.setTargetVelocity(-20000000);
    }
    else if (mode == -2)
    {
      tic3.setTargetVelocity(20000000);
      tic4.setTargetVelocity(-20000000);
    }
    else if (mode == -3)
    {
      tic1.setTargetVelocity(20000000);
      tic2.setTargetVelocity(20000000);
    }
    else if (mode == -4)
    {
      tic1.setTargetVelocity(20000000);
      tic2.setTargetVelocity(20000000);
      tic3.setTargetVelocity(8000000);
      tic4.setTargetVelocity(-8000000);
    }
    else if (mode == -5)
    {
      tic1.setTargetVelocity(8000000);
      tic2.setTargetVelocity(8000000);
      tic3.setTargetVelocity(20000000);
      tic4.setTargetVelocity(-20000000);
    }
}

void backLeft()
{
    tic1.exitSafeStart();
    tic2.exitSafeStart();
    tic3.exitSafeStart();
    tic4.exitSafeStart();

    tic1.energize();
    tic2.energize();
    tic3.energize();
    tic4.energize();
}

void backRight()
{
    tic1.deenergize();
    tic2.deenergize();
    tic3.deenergize();
    tic4.deenergize();
}

void spotRight()
{
    tic1.setTargetVelocity(-10000000);
    tic2.setTargetVelocity(10000000);
    tic3.setTargetVelocity(-10000000);
    tic4.setTargetVelocity(-10000000);
}

void spotLeft()
{
    tic1.setTargetVelocity(10000000);
    tic2.setTargetVelocity(-10000000);
    tic3.setTargetVelocity(10000000);
    tic4.setTargetVelocity(10000000);
}

void die()
{
    tic1.setTargetVelocity(0);
    tic2.setTargetVelocity(0);
    tic3.setTargetVelocity(0);
    tic4.setTargetVelocity(0);
}
