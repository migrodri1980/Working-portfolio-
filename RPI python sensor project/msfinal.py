#!usr/bin/python

import grovepi
import math
import time
import json

# Connect the Grove Temperature & Humidity Sensor Pro to digital port D4
# This example uses the blue colored sensor.
sensor = 4  # The Sensor goes on digital port 4.
# temp_humidity_sensor_type
# Grove Base Kit comes with the blue sensor.
blue = 0    # The Blue colored sensor.
white = 1   # The White colored sensor.

light_sensor = 0  # Connect the Grove Light Sensor to analog port A0

# Connect the LED to digital port D7,D8,D9
blueLed = 7
redLed = 8
greenLed= 6

#light sensor inputs and led declared outputs
grovepi.pinMode(light_sensor,"INPUT")
grovepi.pinMode(blueLed,"OUTPUT")
grovepi.pinMode(redLed,"OUTPUT")
grovepi.pinMode(greenLed,"OUTPUT")


#FILEPATH VARIABLES AND OUTPUT 
path = '/home/pi/Desktop/milestonefinal/'
fileName = 'data'
ext = '.json'
filePathNameExt = path + fileName + ext
outputData = []

dayLight = False #set to false so condition met before recording

#temp range variables. easy to change here
tempLow = 60.0 
tempMed = 85.0
tempHigh = 95.0
humidHigh = 80.0
threshold = 10# Turn on LED once sensor exceeds threshold resistance


def writeJSONFile(path,fileName,data): #defines as path, filename , and data 
    
    with open(filePathNameExt, 'w+') as outfile:
        json.dump(outputData,outfile) #converts python objects into json objects
        
while True:
    try:
        # Get sensor value
        sensor_value = grovepi.analogRead(light_sensor)
        # This example uses the blue colored sensor. 
        # The first parameter is the port, the second parameter is the type of sensor.
        [temp,humidity] = grovepi.dht(sensor,blue) #get temp and humidity from sensor
        temp = ((temp * 9) / 5.) + 32 #conversion from Celsius to Fahrenheit

        # Calculate resistance of sensor in K
        if sensor_value <= 0: #stops divide by zero
            resistance = 0
        
        else:
            resistance = (float)(1023 - sensor_value) * 10 / sensor_value

        if resistance < threshold:
            dayLight = True
            
        else:
            dayLight = False
            
         #TESTS USING PRINT OUTPUT remove # to run test 
        #print("sensor_value = %d resistance = %.2f" %(sensor_value,  resistance)) #test to see
        #print ("dayLight = %s "%(dayLight))

        if  dayLight: #if daylight record temp and humidity 
            if temp > tempLow and temp < tempMed and humidity < humidHigh:
                grovepi.digitalWrite(greenLed,1)#green led on
                #print("cond one")   #to test condition 1 met 

                
            if temp > tempMed and temp < tempHigh and humidity < humidHigh:
                grovepi.digitalWrite(blueLed,1) #blue led on
                #print("cond two") #to test condition2 met 
                
            if temp > tempHigh:
                grovepi.digitalWrite(redLed,1)# red led on
                #print("cond three") # to test condition 3 met 
                  
            if humidity > humidHigh:
                grovepi.digitalWrite(blueLed,1) #blue led on 
                grovepi.digitalWrite(greenLed,1) #green led on
                #print("cond four") #to test condition4 met 
                
            writeJSONFile(path, fileName, outputData) #writes data to desktop name data.json two with temp and humidity data
            outputData.append([temp,humidity])
            print("temp = %.02f F humidity = %.02f%%"%(temp, humidity))
            time.sleep(1800.0) #60 secs per min x 30 min increments= 1800 secs
                       
        else:
            #print ("dayLight = %s "%(dayLight)) #test for daylight condition being false
            time.sleep(1800.0)
        
        #turn off leds after condition met      
        grovepi.digitalWrite(redLed,0)
        grovepi.digitalWrite(greenLed,0)
        grovepi.digitalWrite(blueLed,0)
        
    except IOError:
        print ("Error")

    except KeyboardInterrupt: #detects keyboard interupt
        print ("Program manually terminated")
        break #exit



    