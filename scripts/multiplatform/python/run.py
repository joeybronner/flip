#!/usr/bin/env python

"""Find the currently active window."""

__author__ = 'Joey Bronner'

import sys, time, serial, serial.tools.list_ports

print "Looking for plugged devices"
ports = list(serial.tools.list_ports.comports())
for p in ports:
    print p
print "-------------------"
# COM4 - USB-SERIAL CH340 (COM4)

try:
    arduino = serial.Serial("COM4", timeout=1)
    print(arduino)
except:
    print "No device found, please connect the device to your USB"
    sys.exit()

browsers = ['Internet Explorer', 'Google Chrome']
ides = ['Notepad++', 'Notepad', '| Arduino', 'Android Studio']
designs = ['Photoshop']
multimedias = ['Windows Media Player', 'VLC']
works = ['Outlook']
chats = ['Skype', 'Slack']

def get_active_window():
    active_window_name = None
    active_window_name_test = None
    if sys.platform in ['Windows', 'win32', 'cygwin']:
        import win32gui
        window = win32gui.GetForegroundWindow()
        active_window_name = win32gui.GetWindowText(window)
    elif sys.platform in ['Mac', 'darwin', 'os2', 'os2emx']:
        from AppKit import NSWorkspace
        active_window_name = (NSWorkspace.sharedWorkspace().activeApplication()['NSApplicationName'])
    else:
        print("sys.platform={platform} is unknown. Please report."
            .format(platform=sys.platform))
        print(sys.version)
    return active_window_name

while True:
    time.sleep(1)
    active = get_active_window()
    if any(browser in active for browser in browsers):
        print("Internet")
        arduino.write('g')
    elif any(ide in active for ide in ides):
        print("Development")
        arduino.write('r')
    elif any(design in active for design in designs):
        print("Design")
        arduino.write('r')
    elif any(multimedia in active for multimedia in multimedias):
        print("Design")
        arduino.write('y')
    elif any(work in active for work in works):
        print("Design")
        arduino.write('r')
    elif any(chat in active for chat in chats):
        print("Design")
        arduino.write('g')
    #else:
    #    print("Active window: %s" % str(get_active_window()))
    #    arduino.write('y')
