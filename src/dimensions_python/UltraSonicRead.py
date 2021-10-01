import time
import Jetson.GPIO as GPIO # using RPi.GPIO

class UltraSonicRead:
    
    def ultraRead(self):
        offset = 9.525 # off set between two width sensors in cm
        try:
            GPIO.setmode(GPIO.BOARD)
            PIN_TRIGGER1 = 21   #GPIO 4 pin 7
            PIN_ECHO1 = 19    #GPIO 17 pin 11
            
            PIN_TRIGGER2 = 18
            PIN_ECHO2 = 16

            PIN_TRIGGER3 = 13
            PIN_ECHO3 = 7

            # Setup sensor 1
            GPIO.setup(PIN_TRIGGER1, GPIO.OUT)
            GPIO.setup(PIN_ECHO1, GPIO.IN)
            
            # Setup sensor 2
            GPIO.setup(PIN_TRIGGER2, GPIO.OUT)
            GPIO.setup(PIN_ECHO2, GPIO.IN)

            # Setup sensor 3
            GPIO.setup(PIN_TRIGGER3, GPIO.OUT)
            GPIO.setup(PIN_ECHO3, GPIO.IN)
            
            #Sensor 1
            GPIO.output(PIN_TRIGGER1, GPIO.LOW)
            print('Scanning Sensor 1')
            time.sleep(2)
            
            print('Calculating distance')
            GPIO.output(PIN_TRIGGER1, GPIO.HIGH)
            time.sleep(0.00001)
            
            GPIO.output(PIN_TRIGGER1, GPIO.LOW)
            
            while GPIO.input(PIN_ECHO1) == 0:
                pulse_start_time = time.time()
            while GPIO.input(PIN_ECHO1) == 1:
                pulse_end_time = time.time()
            
            pulse_duration = pulse_end_time - pulse_start_time
            distance1 = round(pulse_duration * 17150, 2)
            print("   Distance 1:",distance1,"cm")
            
            #sensor 2
            GPIO.output(PIN_TRIGGER2, GPIO.LOW)
            print('Scanning Sensor 2')
            time.sleep(2)
            
            print('Calculating distance')
            GPIO.output(PIN_TRIGGER2, GPIO.HIGH)
            time.sleep(0.00001)
            
            GPIO.output(PIN_TRIGGER2, GPIO.LOW)
            
            while GPIO.input(PIN_ECHO2) == 0:
                pulse_start_time = time.time()
            while GPIO.input(PIN_ECHO2) == 1:
                pulse_end_time = time.time()
            
            pulse_duration = pulse_end_time - pulse_start_time
            distance2 = round(pulse_duration * 17150, 2)
            print("   Distance 2:",distance2,"cm")
            
            #Sensor 3
            GPIO.output(PIN_TRIGGER3, GPIO.LOW)
            print('Scanning Sensor 3')
            time.sleep(2)
            
            print('Calculating distance')
            GPIO.output(PIN_TRIGGER3, GPIO.HIGH)
            time.sleep(0.00001)
            
            GPIO.output(PIN_TRIGGER3, GPIO.LOW)
            
            while GPIO.input(PIN_ECHO3) == 0:
                pulse_start_time = time.time()
            while GPIO.input(PIN_ECHO3) == 1:
                pulse_end_time = time.time()
            
            pulse_duration = pulse_end_time - pulse_start_time
            distance3 = round(pulse_duration * 17150, 2)
            print("   Distance 3:",distance3,"cm")
            
        except:
            print("error with ultrasonic read")
        finally:
            width = round(distance3 + distance2 + offset,2)
            height = round(distance1,2)
            
            GPIO.cleanup()
            return [width, height]

sensor = UltraSonicRead()
measure = sensor.ultraRead()
print(measure[0])
print(measure[1])
