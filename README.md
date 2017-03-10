# SAPoes

## Openspace Employee Status

*Switch on a RGB LED following the current user window (e.g. Slack=Green ; SublimeText=Red ; Word=Orange)*

<img src="resources/status.png" />

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
