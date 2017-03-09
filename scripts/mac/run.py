#!/usr/bin/python
import sched, time, serial, sys
from AppKit import NSWorkspace

import time

try:
	ser = serial.Serial("COM4",timeout=1)
	print(ser)
except:
	print "No device found, please connect the device to your USB"
	sys.exit()

def executeSomething():
	activeAppName = NSWorkspace.sharedWorkspace().activeApplication()['NSApplicationName']
	print activeAppName
	time.sleep(1)

while True:
	executeSomething()