#!/usr/bin/env python3

from smbus2 import SMBus, i2c_msg
import rospy
from geometry_msgs.msg import Twist

class TicI2C(object):
  def __init__(self, bus, address):
    self.bus = bus
    self.address = address
 
  # Sends the "Exit safe start" command.
  def exit_safe_start(self):
    command = [0x83]
    write = i2c_msg.write(self.address, command)
    self.bus.i2c_rdwr(write)
 
  # Sets the target position.
  #
  # For more information about what this command does, see the
  # "Set target position" command in the "Command reference" section of the
  # Tic user's guide.
  def set_target_position(self, target):
    command = [0xE0,
      target >> 0 & 0xFF,
      target >> 8 & 0xFF,
      target >> 16 & 0xFF,
      target >> 24 & 0xFF]
    write = i2c_msg.write(self.address, command)
    self.bus.i2c_rdwr(write)
 
  # Gets one or more variables from the Tic.
  def get_variables(self, offset, length):
    write = i2c_msg.write(self.address, [0xA1, offset])
    read = i2c_msg.read(self.address, length)
    self.bus.i2c_rdwr(write, read)
    return list(read)
 
  # Gets the "Current position" variable from the Tic.
  def get_current_position(self):
    b = self.get_variables(0x22, 4)
    position = b[0] + (b[1] << 8) + (b[2] << 16) + (b[3] << 24)
    if position >= (1 << 31):
      position -= (1 << 32)
    return position

global vel
global ang

def move(vel, ang):
  position1 = tic1.get_current_position()
  position2 = tic2.get_current_position()
  if(position1 < max_position and vel > 0 and ang > 0):
    new_target = position1 + 100
    tic1.exit_safe_start()
    tic2.exit_safe_start()
    tic1.set_target_position(new_target)
    tic2.set_target_position(new_target)
    print("Setting target position to {}.".format(new_target));
  elif(position1 > min_position and vel > 0 and ang < 0):
    new_target = position1 - 100
    tic1.exit_safe_start()
    tic2.exit_safe_start()
    tic1.set_target_position(new_target)
    tic2.set_target_position(new_target)
    print("Setting target position to {}.".format(new_target));

def listener():
  rospy.init_node('cmd_vel_listener')
  rospy.Subscriber("/cmd_vel", Twist, callback)
  rospy.spin()


def callback(msg):
  vel = msg.linear.x
  ang = msg.angular.z

  print("Lineqar x = %f. Angular z = %f"%(vel, ang))
  move(vel, ang)

 
# Open a handle to "/dev/i2c-3", representing the I2C bus.
bus = SMBus(1)
 
# Select the I2C address of the Tic (the device number).
address1 = 14
address2 = 15
 
global max_position
max_position = 5000
global min_position
min_position = 0

global tic1
tic1 = TicI2C(bus, address1)
global tic2
tic2 = TicI2C(bus, address2)
global position1
#position1 = tic1.get_current_position()
global position2
#position2 = tic2.get_current_position()
listener()
print("Current position1 is {}.".format(position1))
print("Current position2 is {}.".format(position2))
 

