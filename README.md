# ArduWatchOS-v1.0
A public operating system for an Arduino smartwatch with an 7 pin ssd1331 display and for use with iOS

The first version of the ArduWatchOS software. 
This version works only with ssd1331 displays. 
It has only been tested with the hm-10 bluetooth module. 
To upload the code download arduino ide, connect your arduino, copy and paste this code into the ide, 
select the proper COM port, and hit upload.
Only use the 7 pin ssd1331 display.

To connect the hm-10 bluetooth module to the arduino, connect the VCC to the 3V pin, the GND to the GND pin,
connect RX on the hm-10 to the TX on the arduino, connect TX on the hm-10 to the RX on the arduino. I know, the RX and TX is weird, but 
just do it like that. I would connect RX and TX with jumper pins that could be removed, as you can not upload code with anything connected
to the Arduino RX and TX. If you have any problems uploading code, check the hm-10 to ensure that nothing is connected to RX and TX.


To connect the display to the arduino, connect pins like so:

GND to GND
VCC to 3.3V
SCL to D13
SDA to D11
RES to D9
DC to D8
CS to D10
