#!/usr/bin/env python

"""Find the currently active window."""

import logging
import sys
import time
import serial

#try:
#    ser = serial.Serial("COM4",timeout=1)
#    print(ser)
#except:
#    print "No device found, please connect the device to your USB"
#    sys.exit()

logging.basicConfig(format='%(asctime)s %(levelname)s %(message)s',
                    level=logging.DEBUG,
                    stream=sys.stdout)

win_browsers = ['Internet Explorer', 'Google Chrome']
win_ides = ['Notepad++', 'Notepad', '| Arduino', 'Android Studio']
win_design = ['Photoshop']
win_multimedia = ['Windows Media Player', 'VLC']
win_work = ['Outlook']
win_chat = ['Skype', 'Slack']

def get_active_window():
    """
    Get the currently active window.

    Returns
    -------
    string :
        Name of the currently active window.
    """
    import sys
    active_window_name = None
    active_window_name_test = None
    if sys.platform in ['linux', 'linux2']:
        active_window_name = 'Linux'
        # Alternatives: http://unix.stackexchange.com/q/38867/4784
        #try:
        #    import wnck
        #except ImportError:
        #    logging.info("wnck not installed")
        #    wnck = None
        #if wnck is not None:
        #    screen = wnck.screen_get_default()
        #    screen.force_update()
        #    window = screen.get_active_window()
        #    if window is not None:
        #        pid = window.get_pid()
        #        with open("/proc/{pid}/cmdline".format(pid=pid)) as f:
        #            active_window_name = f.read()
        #else:
        #    try:
        #        from gi.repository import Gtk, Wnck
        #        gi = "Installed"
        #    except ImportError:
        #        logging.info("gi.repository not installed")
        #        gi = None
        #    if gi is not None:
        #        Gtk.init([])  # necessary if not using a Gtk.main() loop
        #        screen = Wnck.Screen.get_default()
        #        screen.force_update()  # recommended per Wnck documentation
        #        active_window = screen.get_active_window()
        #        pid = active_window.get_pid()
        #        with open("/proc/{pid}/cmdline".format(pid=pid)) as f:
        #            active_window_name = f.read()
    elif sys.platform in ['Windows', 'win32', 'cygwin']:
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
    else:
        print("Active window: %s" % str(get_active_window()))

