#include "TinyGPS++.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#define SIM800_TX_PIN 2
#define SIM800_RX_PIN 3
#define GPS_TX_PIN 8
#define GPS_RX_PIN 9

SoftwareSerial GPS(GPS_TX_PIN, GPS_RX_PIN);
SoftwareSerial SIM(SIM800_TX_PIN, SIM800_RX_PIN);
TinyGPSPlus gps;

String APN = "internet";
String APN_USER = "true";
String APN_PASS = "true";
String URL = "http://miliohm.com/getData.php?data=";  //URL for HTTP-POST-REQUEST
String URLS = "http://34.87.122.165/sims/web/call_apis.php?true=1";
int ran;

// Used baud rates (define based on used modules)
static const uint32_t SimBaudrate = 9600;
static const uint32_t GPSBaud = 9600;
static const uint32_t SerialBaudrate = 9600;

void setup() {

  Serial.begin(SerialBaudrate);
  GPS.begin(GPSBaud);
  SIM.begin(SimBaudrate);
  SIM.listen();

  Serial.println("Waiting for init");
  // Wait few seconds so that module is able to take AT commands
  delay(5000);
  Serial.println("Init... waiting until module has connected to network");

  // Start AT communication. This sets auto baud and enables module to send data
  SIM.println("AT");
  // Wait until module is connected and ready

}

void loop()
{
  //  gsm_sendhttp();
  while (GPS.available()) {
    gps.encode(GPS.read());
  }
  if (gps.location.isUpdated()) {
    Serial.println("==========================================");
    Serial.println("Satellite Count:" + String(gps.satellites.value()));
    Serial.println("Latitude:" + String(gps.location.lat(), 6));
    Serial.println("Longitude:" + String(gps.location.lng(), 6));
    Serial.println("Speed MPH:" + String(gps.speed.mph()));
    Serial.println("Altitude Feet:" + String(gps.altitude.feet()));
    Serial.println("==========================================");
  }
}


void gsm_sendhttp() {


  while (GPS.available()) //While there are characters to come from the GPS
  {
    gps.encode(GPS.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {

    Serial.println("");
    Serial.println("Initializing...");
    delay(1000);
    SIM.println("AT");
    runsl();
    delay(3000);
    SIM.println("AT+CSQ");    //Signal quality test, value range is 0-31 , 31 is the best
    runsl();
    delay(1000);
    SIM.println("AT+CCID");   //Read SIM information to confirm whether the SIM is plugged
    runsl();
    delay(1000);
    SIM.println("AT+CREG?");  //Check whether it has registered in the network
    runsl();
    delay(1000);
    SIM.println("AT+SAPBR=3,1,Contype,GPRS");
    runsl();
    delay(100);
    SIM.println("AT+SAPBR=3,1,APN," + APN);
    runsl();
    delay(100);
    SIM.println("AT+SAPBR=3,1,USER," + APN_USER); //Comment out, if you need username
    runsl();
    delay(100);
    SIM.println("AT+SAPBR=3,1,PWD," + APN_PASS); //Comment out, if you need password
    runsl();
    delay(100);
    SIM.println("AT+SAPBR=1,1");
    runsl();
    delay(1000);
    SIM.println("AT+SAPBR=2,1");
    runsl();
    delay(2000);
    SIM.println("AT+HTTPINIT");
    runsl();
    delay(100);
    SIM.println("AT+HTTPPARA=CID,1");
    runsl();
    delay(100);
    SIM.println("AT+HTTPPARA=URL," + URLS + "&latitude=" + String(gps.location.lat(), 6) + "&longitude=" + String(gps.location.lng(), 6));
    runsl();
    delay(1000);
    SIM.println("AT+HTTPACTION=0");
    runsl();
    delay(3000);
    SIM.println("AT+HTTPREAD");
    runsl();
    delay(100);
    SIM.println("AT+HTTPTERM");
    runsl();

    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("==========================================");
    Serial.println("Satellite Count:" + String(gps.satellites.value()));
    Serial.println("Latitude:" + String(gps.location.lat(), 6));
    Serial.println("Longitude:" + String(gps.location.lng(), 6));
    Serial.println("Speed MPH:" + String(gps.speed.mph()));
    Serial.println("Altitude Feet:" + String(gps.altitude.feet()));
    Serial.println("==========================================");
  }
}

//Print GSM Status
void runsl() {
  while (SIM.available()) {
    Serial.write(SIM.read());
  }
}
