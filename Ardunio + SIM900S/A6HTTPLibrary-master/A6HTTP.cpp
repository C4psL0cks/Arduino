/*
A6HTTP.cpp - HTTP Post library for Arduino.
2016/09/29
Made by Tobias Koops from MicroEnergyInternational
Based on code from Andreas Spiess
License: Opensource
http://gitlab.koukei.de/Tobias/A6HTTPLibrary.git
*/

#include "Arduino.h"


#include "A6HTTP.h"



A6HTTP::A6HTTP(String APN, String host, String path, int port, String Content_Type)
{
 OK = 1;
 NOTOK = 2;
 TIMEOUT = 3;
 RST = 10; //This is your Resetpin
 RX = 4; //This is your RX-Pin on arduino
 TX = 3; //This is your TX-Pin on arduino
 A6baud = 9600; //Baudrate between Arduino and A6
 SERIALTIMEOUT = 3000;

 A6board= new SoftwareSerial(RX, TX);
 _APN = APN;
_host = host;
 _path = path;
 _port = port;
_Content_Type = Content_Type;
}

void A6HTTP::begin()
{
A6board->begin(A6baud);
Serial.begin(115200);
end_c[0]=0x1a;
end_c[1]='\0'; 
Serial.println("Start setup");
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  delay(2000);
  digitalWrite(RST, LOW);
  delay(500);
  if (A6begin() != OK) {
    Serial.println("Error A6begin");
    while (1 == 1);
  }
  Serial.println("Setup done");
}

bool A6HTTP::HttpPost(String body)
{
	Serial.println("Start post function");
  int bodyLength = body.length();
  Serial.println(bodyLength);
  A6command("AT+CGATT?", "OK", "yy", 20000, 2);
  A6command("AT+CGATT=1", "OK", "yy", 20000, 2);
  A6board->println("AT+CSQ"); //Signal Quality
  delay(200);
  ShowSerialData();
  A6command("AT+CGDCONT=1,\"IP\",\""+ _APN +"\"", "OK", "yy", 20000, 2); //bring up wireless connection
  A6command("AT+CGACT=1,1", "OK", "yy", 10000, 2);
 // A6command("AT+CIFSR", "OK", "yy", 20000, 2);  
  delay(1000);
  A6command("AT+CIPSTART=\"TCP\",\"" + _host + "\","+ _port, "CONNECT OK", "yy", 25000, 2); //start up the connection
  A6command("AT+CIPSTATUS", "OK", "yy", 10000, 2);
  A6command("AT+CIPSEND", ">", "yy", 10000, 1); //begin send data to remote server
  delay(500);
  A6board->print("POST "+ _path);
  A6board->print(" HTTP/1.1");
  A6board->print("\r\n");
  A6board->print("HOST: ");
  A6board->print(_host);
  A6board->print("\r\n");
  A6board->print("Content-Length: " + String(bodyLength));
  A6board->print("\r\n");
  A6board->print("Content-Type: " + _Content_Type);
  A6board->print("\r\n");
  A6board->print("\r\n");
  A6board->print(body);
  A6command(end_c, "OK", "yy", 30000, 1); //begin send data to remote server
  unsigned long   entry = millis();
  A6command("AT+CIPSTATUS", "OK", "yy", 10000, 2);
  delay(1000);
  A6command("AT+CIPCLOSE", "OK", "yy", 15000, 1); 
  delay(100);
}

byte  A6HTTP::A6waitFor(String response1, String response2, int timeOut) {
  unsigned long entry = millis();
  int count = 0;
  String reply = A6read();
  byte retVal = 99;
  do {
    reply = A6read();
    if (reply != "") {
      Serial.print((millis() - entry));
      Serial.print(" ms ");
      Serial.println(reply);
    }
  } while ((reply.indexOf(response1) + reply.indexOf(response2) == -2) && millis() - entry < timeOut );
  if ((millis() - entry) >= timeOut) {
    retVal = TIMEOUT;
  } else {
    if (reply.indexOf(response1) + reply.indexOf(response2) > -2) retVal = OK;
    else retVal = NOTOK;
  }
  //  Serial.print("retVal = ");
  //  Serial.println(retVal);
  return retVal;
}

byte  A6HTTP::A6command(String command, String response1, String response2, int timeOut, int repetitions) {
  byte returnValue = NOTOK;
  byte count = 0;
  while (count < repetitions && returnValue != OK) {
    A6board->println(command);
    Serial.print("Command: ");
    Serial.println(command);
    if (A6waitFor(response1, response2, timeOut) == OK) {
      //     Serial.println("OK");
      returnValue = OK;
    } else returnValue = NOTOK;
    //Serial.println("NOTOK" + count);
    count++;
  }
  return returnValue;
}




void  A6HTTP::A6input() {
  String hh;
  char buffer[100];
  while (1 == 1) {
    if (Serial.available()) {
      hh = Serial.readStringUntil('\n');
      hh.toCharArray(buffer, hh.length() + 1);
      if (hh.indexOf("ende") == 0) {
        A6board->write(end_c);
        Serial.println("ende");
      } else {
        A6board->write(buffer);
        A6board->write('\n');
      }
    }
    if (A6board->available()) {
      Serial.write(A6board->read());
    }
  }
}


bool  A6HTTP::A6begin() {
  A6board->println("AT+CREG?");
  byte hi = A6waitFor("1,", "5,", 1500);  // 1: registered, home network ; 5: registered, roaming
  while ( hi != OK) {
    A6board->println("AT+CREG?");
    hi = A6waitFor("1,", "5,", 1500);
  }

  if (A6command("AT&F0", "OK", "yy", 5000, 2) == OK) {   // Reset to factory settings
    if (A6command("ATE0", "OK", "yy", 5000, 2) == OK) {  // disable Echo
      if (A6command("AT+CMEE=2", "OK", "yy", 5000, 2) == OK) return OK;  // enable better error messages
      else return NOTOK;
    }
  }
}

void  A6HTTP::ShowSerialData()
{
  unsigned long entry = millis();
  while ( A6board->available() != 0 && millis() - entry < SERIALTIMEOUT)
    Serial.println(A6board->readStringUntil('\n'));
}

String A6HTTP::A6read() {
  String reply = "";
  if (A6board->available())  {
    reply = A6board->readString();
  }
  //  Serial.print("Reply: ");
  //  Serial.println(reply);
  return reply;
}
