# flip

*Switch on a RGB LED (e.g. Slack=Green ; SublimeText=Red ; Word=Orange) following...*  
 *- option1, the current user window*  
 *- option2, the skype status (away, busy, available, in a meeting, presenting...)*  
 *- option3, manually with a physical button*  

<img src="resources/status.png" />  

### Install

...

### Launch

## OSX

First install dependencies 

	sudo easy_install pip
	sudo pip install paho-mqtt
	sudo pip install pyserial

Now, run the script 

	python run.py

### Electronics

 - 1x Arduino Nano
 - 1x LED RGB

### 3D Printing

 - [Box for Arduino Nano](http://www.thingiverse.com/thing:131894)
 - Avatar of something else in which we can put the LED

 ### Useful links

 - [Github | MQTT-ESP8266-CLOUDMQTT.COM/example.ino](https://github.com/Protoneer/MQTT-ESP8266-CLOUDMQTT.COM/blob/master/example.ino)

