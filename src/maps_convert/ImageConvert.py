#!/usr/bin/env python

from PIL import Image
import glob
import datetime
import time
from pytz import timezone
import rospy
import subprocess
from geometry_msgs.msg import Twist

def convert_image():
  tz = timezone('US/Eastern')

  for file in glob.glob("*.pgm"):
      im = Image.open(file)
      print(file)

    # Date and Time stamp for the map
      d = datetime.datetime.now()
      t = datetime.datetime.now(tz)
      fileName = "airMap_" + d.strftime('%m-%d-%Y') + "_" + t.strftime('%H-%M-%S') + ".png"

      img_png = im
      img_png.save(fileName)

def callback(msg):
  if(msg.linear.x == 7):
    subprocess.call('/home/spiderbotgold/SeniorDesignSpiderBot/mapsave.sh', shell = True)
    time.sleep(5)
    convert_image()
    print("map saved")


rospy.init_node('convert_map')
rospy.Subscriber("/cmd_vel", Twist, callback)
rospy.spin()



