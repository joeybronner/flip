# SAPoes

## Openspace Employee Status

*Switch on a RGB LED following the current user window (e.g. Slack=Green ; SublimeText=Red ; Word=Orange)*

<img src="resources/status.png" />

### List of applications detected

##### Browser
 - Google Chrome
 - Internet Explorer

##### Development IDEs
 - Notepad++
 - Arduino IDE

### Install

#### Windows

##### Step 1

Following the user platform, run the win32gui corresponding.
e.g. Python2.7 + 64 bits = pywin32-216.win-amd64-py2.7.exe

##### Step 2

    python C:\Python27\Scripts\pywin32_postinstall.py -install

#### Mac OSX

Homebrew not installed?

    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

apt-get command not found?

    brew install python
	brew install apt-get

No module named serial?

	pip install pyserial

No module named AppKit?

	pip install pyobjc

Open a terminal

	sudo apt-get install python3-pip
	pip install pyserial
	sudo pip3 install pyserial

#### Linux

*Not supported yet.*

### Useful links

 - http://www.lfd.uci.edu/~gohlke/pythonlibs/#pywin32
 - https://sourceforge.net/projects/pywin32/files/pywin32/Build216/
 - http://docs.activestate.com/activepython/3.2/pywin32/win32gui.html
