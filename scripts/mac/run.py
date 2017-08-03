#!/usr/bin/python
import time, serial, sys, paho.mqtt.client as mqtt
from AppKit import NSWorkspace

#try:
#	ser = serial.Serial("COM12",timeout=1)
#	print(ser)
#except:
#	print "No device found, please connect the device to your USB"
#	sys.exit()

mqttc = mqtt.Client("flip-joey")
mqttc.username_pw_set("flip-joey", "helloworld")
mqttc.connect("m10.cloudmqtt.com", 17316, 60)
mqttc.loop_start()    

def getCurrentWindow():
	activeAppName = NSWorkspace.sharedWorkspace().activeApplication()['NSApplicationName']
	print activeAppName
	mqttc.publish("flip/devices/1/status","g")
	time.sleep(1)

while True:
	getCurrentWindow()