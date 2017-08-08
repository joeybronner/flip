#!/usr/bin/python
import time, sys, paho.mqtt.client as mqtt
from AppKit import NSWorkspace

# Here, get device configuration from HTTP API

mqttc = mqtt.Client("flip-joey")
mqttc.username_pw_set("flip-joey", "helloworld")
mqttc.connect("m10.cloudmqtt.com", 17316, 60)
mqttc.loop_start()    

def getCurrentWindow():
	activeAppName = NSWorkspace.sharedWorkspace().activeApplication()['NSApplicationName']
	print activeAppName
	if activeAppName == "Google Chrome":
		mqttc.publish("flip/devices/1/status","g")
	if activeAppName == 'Sublime Text':
		mqttc.publish("flip/devices/1/status","r")
	if activeAppName == 'Arduino':
		mqttc.publish("flip/devices/1/status","y")
	
	time.sleep(1)

while True:
	getCurrentWindow()