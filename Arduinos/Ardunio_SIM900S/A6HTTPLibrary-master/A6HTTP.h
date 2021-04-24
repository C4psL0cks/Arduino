/*
A6HTTP.cpp - HTTP Post library for Arduino.
2016/09/29
Made by Tobias Koops from MicroEnergyInternational
Based on code from Andreas Spiess
License: Opensource
http://gitlab.koukei.de/Tobias/A6HTTPLibrary.git
*/

#ifndef A6HTTP_h
#define A6HTTP_h

#include "Arduino.h"
#include "String.h"
#include "SoftwareSerial.h"

class A6HTTP
{
  public:
    A6HTTP(String APN, String host, String path, int port, String Content_Type);
    bool HttpPost(String body);
    void begin();
  
  private:
    byte A6waitFor(String response1,String response2, int timeOut);
    byte A6command(String command, String response1, String response2, int timeOut, int repetitions);
    void A6input();
    bool A6begin();
    void ShowSerialData();
    String A6read();

  byte OK;
 	byte NOTOK;
 	byte TIMEOUT;
 	byte RST;
 	byte RX;
 	byte TX;
 	int A6baud;
 	int SERIALTIMEOUT;
 	char end_c[2];

    String _APN;
	String _host;
	String _path;
	int _port;
	String _Content_Type;

	 SoftwareSerial* A6board;
	
};

#endif
