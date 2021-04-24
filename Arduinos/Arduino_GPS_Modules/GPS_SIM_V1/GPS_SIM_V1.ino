/*
  For more Information visit www.aeq-web.com?ref=arduinoide
  SIM800 5v/4v ↔ Arduino 5V
  SIM800 GND (either one) ↔ Arduino GND
  SIM800 SIM_TXD ↔ Arduino D8
  SIM800 SIM_RXD ↔ Arduino D7
*/

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>
#define SIM800_TX_PIN 2
#define SIM800_RX_PIN 3
#define GPS_TX_PIN 10
#define GPS_RX_PIN 11

SoftwareSerial SIMS(SIM800_TX_PIN, SIM800_RX_PIN);
SoftwareSerial GPSS(GPS_TX_PIN, GPS_RX_PIN);
TinyGPSPlus gps;

String APN = "internet";
String APN_USER = "true";
String APN_PASS = "true";
String URL = "http://miliohm.com/getData.php?data=";  //URL for HTTP-POST-REQUEST
int ran;
//
static const unsigned long frequency = 15000;
static long maxResponseTime = 30000;
String responseString;
unsigned long previous = 0;
unsigned long beltTime;

void setup() {

  Serial.begin(9600);
  SIMS.begin(9600);
  GPSS.begin(9600);
  Serial.println("Initializing...");
  //  SIMS.println("AT");
  //  waitUntilResponse("OK");
  SIMS.println("AT");        // Once the handshake test is successful, it will back to OK
  delay(1000);
  SIMS.println("AT+CSQ");    //Signal quality test, value range is 0-31 , 31 is the best
  delay(1000);
  SIMS.println("AT+CCID");   //Read SIM information to confirm whether the SIM is plugged
  delay(1000);
  SIMS.println("AT+CREG?");  //Check whether it has registered in the network
  delay(1000);
  Serial.println("Setup Complete!");
}

void loop() {
  while (SIMS.available()) {
    Serial.write(SIMS.read());
  }
}

void waitUntilResponse(String response) {
  beltTime = millis();
  responseString = "";
  String totalResponse = "";
  while (responseString.indexOf(response) < 0 && millis() - beltTime < maxResponseTime) {
    readResponse();
    totalResponse = totalResponse + responseString;
    Serial.println(responseString);
  }

  if (totalResponse.length() <= 0) {
    Serial.println("No response from the module. Check wiring, SIM-card and power!");
    delay(30000);
    exit(0); // No way to recover
  }
  else if (responseString.indexOf(response) < 0) {
    Serial.println("Unexpected response from the module");
    Serial.println(totalResponse);
    delay(30000);
    exit(0); // No way to recover
  }
}

void readResponse() {
  responseString = "";
  while (responseString.length() <= 0 || !responseString.endsWith("\n")) {
    tryToRead();
    if (millis() - beltTime > maxResponseTime) {
      return;
    }
  }
}

void tryToRead() {
  while (SIMS.available()) {
    char c = SIMS.read();  //gets one byte from serial buffer
    responseString += c; //makes the string readString
  }
}
