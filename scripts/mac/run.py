#!/usr/bin/python
import sched, time, serial, sys
from AppKit import NSWorkspace

try:
	ser = serial.Serial("COM4",timeout=1)
	print(ser)
except:
	print "No device found, please connect the device to your USB"
	sys.exit()

def getCurrentWindow():
	activeAppName = NSWorkspace.sharedWorkspace().activeApplication()['NSApplicationName']
	print activeAppName
	time.sleep(1)

while True:
	getCurrentWindow()