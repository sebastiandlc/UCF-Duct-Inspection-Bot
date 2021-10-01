#!/usr/bin/env python3

# Odometry data
# How to recognize a turn
# When turn run the code
# Ros topic to exit and save

import DocWriteXML as xml
import MapObject as mapobj
import UltraSonicRead as US
import random
import time

def randDirec():
    direc = ["Left", "Right"]
    return direc[random.randint(0,1)]   


print("writing to xml:")

go = True
num = 1
width = []
height = []
vert = False # variable for if there is vertical section
objList = []
turn = False
vertLen = 0

lastMeasurement = 1000

sensor = US.UltraSonicRead()

# While not stoped from topic run
while(go):
    print("Direction number ",num)
    measurement = sensor.ultraRead()
    time.sleep(5)  # wait 5 seconds
    
    width.append(measurement[0])
    height.append(measurement[1])
    
    if(measurement[1] - lastMeasurement > 30):
        vert = True
        vertLen = 20
    
    lastMeasurement = measurement[1] # set the lastmeasurement to the current height
    
# If turn detected then run this
    if(turn): # MapObject(directionNum,vertical,verticalPos,length,width,height,turnDirection)
        widthAvg = round(sum(width) / len(width),2)
        heightAvg = round(sum(height) / len(height),2)
        turnDirection = randDirec()
        
        obj = mapobj.MapObject(num,vert,0,vertLen,widthAvg,heightAvg,turnDirection)
        objList.append(obj)
        num+=1
        vert = False
        turn = False
        width = []
        height = []
        
    if(len(width) == 2):
        turn = True
    if(num == 5):
        go = False

# Final Exit code
map = xml.DocWriteXML()
map.toXML(objList)

print("done")
