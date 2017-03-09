#!/usr/bin/python
import sched, time, serial
from AppKit import NSWorkspace

import time

ser = serial.Serial("COM4",timeout=1)
print(ser)

def executeSomething():
	activeAppName = NSWorkspace.sharedWorkspace().activeApplication()['NSApplicationName']
	print activeAppName
	time.sleep(1)

while True:
	executeSomething()