Readme - HTTP Post library for Arduino.
2016/09/29
Made by Tobias Koops from MicroEnergyInternational
Based on code from Andreas Spiess
License: Opensource
http://gitlab.koukei.de/Tobias/A6HTTPLibrary.git

This is a library for the A6-GSMChip by Ai-Thinkers.

Standart settings:
RX(Arduino): PIN 4
TX(Arduino): PIN 3
Reset: PIN 10 (needed for correct initialization)
Baudrate: 9600 (Arduino - A6)
Baudrate: 112500 (Arduino - PC)
These can be changed in A6HTTP.cpp

In A6HTTP_Example I used the free service from posttestserver.com of posting something on it and beeing able to see it afterwards.

IMPORTANT WIRING (see pictures for details):

1.
For correct initialization the A6 is reseted on startup. In order for that to work the reset Pins cannot be connected directely.
First you need to permanently connect PWR and VCC on the module. This will also turn on the module, once it gets power.
Then ýou will need a transistor or mosfet, between the reset pins. I used an IRF3205 N-channel mosfet.
Connect like this: 
Gate - Arduino reset(Pin10)
Drain - A6 reset
Source - Ground
This will allow the A6reset to be jumped with ground, whenever the arduino pin 10 is in state HIGH.

2. 
The A6 has 3.3V logiclevel. Which means you need to shift the arduino's 5V of its TX Pin (3) to a suitable voltage.
I used a voltagedivider consisting of 3 identical resistors. 
However you can directly connect RX of arduino and TX of A6 because arduino will accept 3.3V level high's. 

3.
I had the problem of voltage drops whenever the A6 was sending. Altough my power source supplied enough current, it seems that the cables leading to the A6 may be too thin.
I managed to solve this by soldering 2 1mF capacitors (because i dind't have larger ones) directly onto the board between VCC and Ground.


If you set up everything the right way, you should be able to use this example just as it is.


