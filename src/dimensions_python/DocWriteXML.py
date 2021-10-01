# Class for writing to xml file
# view xmlExample.xml for example on formatting

import datetime
import MapObject
from lxml import etree as ET

class DocWriteXML:

    def toXML(self, objList):
        map = ET.Element('map')
        for obj in objList:
            subCatName = 'direction' + str(obj.directionNum)
            direction = ET.SubElement(map, subCatName)
            length = ET.SubElement(direction, 'length')
            width = ET.SubElement(direction, 'width')
            height = ET.SubElement(direction, 'height')
            turnDirection = ET.SubElement(direction,'turnDirection')

            # If script detects possible vertical section, stores likely
            # position based off length
            if obj.vertical:
                vertical = ET.SubElement(direction, 'vertical')
                vertical.set('position',str(obj.verticalPos))

            # measured from ultrasonic sensors
            length.text = str(obj.length)
            width.text = str(obj.width)
            height.text = str(obj.height)
            turnDirection.text = obj.turnDirection

        # calculate time stamp for map name
        d = datetime.datetime.now()
        filename = "map_" + d.strftime('%m-%d-%Y') + "_" + d.strftime('%H-%M') + ".xml"

        # saves actual map
        mymap = ET.tostring(map, pretty_print=True)
        myfile = open(filename,"wb")
        myfile.write(mymap)
