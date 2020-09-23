/*
A6HTTP_example - HTTP Post library for Arduino.
2016/09/29
Made by Tobias Koops from MicroEnergyInternational
Based on code from Andreas Spiess
License: Opensource
http://gitlab.koukei.de/Tobias/A6HTTPLibrary.git

----------ATTENTION--------------
Standart settings:
RX(Arduino): PIN 4
TX(Arduino): PIN 3
Reset: PIN 10 (needed for correct initialization)
Baudrate: 9600 (Arduino - A6)
Baudrate: 112500 (Arduino - PC)
You may change these settings in A6HTTP.cpp
*/

#include <A6HTTP.h>
A6HTTP myA6("MYAPN", "www.posttestserver.com", "/post.php", 80, "application/x-www-form-urlencoded");

//Syntax: A6HTTP myA6("APN", "host", "path", port, "Content-Type");
//Find your APN for your provider online
//posttestserver.com lets you post anything for free and have a look at it afterwards.

void setup() {
  myA6.begin();
  //initialize HTTP library. 

  myA6.HttpPost("This is a Test");
  //Syntax: myA6.HttpPost("body");
  //body is the actual posted data matching your defined Content-Type
}

void loop() {
//empty
}
