/*
  For more Information visit www.aeq-web.com?ref=arduinoide
  SIM800 5v/4v ↔ Arduino 5V
  SIM800 GND (either one) ↔ Arduino GND
  SIM800 SIM_TXD ↔ Arduino D8
  SIM800 SIM_RXD ↔ Arduino D7
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#define SIM800_TX_PIN 2
#define SIM800_RX_PIN 3
SoftwareSerial SIM(SIM800_TX_PIN, SIM800_RX_PIN);
String APN = "internet";
String APN_USER = "true";
String APN_PASS = "true";
String URL = "http://miliohm.com/getData.php?data=";  //URL for HTTP-POST-REQUEST
int ran;

void setup() {

  Serial.begin(9600);
  SIM.begin(9600);
  Serial.println("Initializing...");
  delay(1000);
  SIM.println("AT");        // Once the handshake test is successful, it will back to OK
  delay(1000);
  SIM.println("AT+CSQ");    //Signal quality test, value range is 0-31 , 31 is the best
  delay(1000);
  SIM.println("AT+CCID");   //Read SIM information to confirm whether the SIM is plugged
  delay(1000);
  SIM.println("AT+CREG?");  //Check whether it has registered in the network
  delay(1000);
  Serial.println("Setup Complete!");
}

void loop() {

  ran = random(300);
  Serial.println(ran);
  gsm_sendhttp(); //Start the GSM-Modul and start the transmisson
  delay(30000); //Wait one minute
}
void gsm_sendhttp() {
  SIM.println("AT");
  runsl();//Print GSM Status an the Serial Output;
  delay(4000);
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
  SIM.println("AT+SAPBR =1,1");
  runsl();
  delay(100);
  SIM.println("AT+SAPBR=2,1");
  runsl();
  delay(2000);
  SIM.println("AT+HTTPINIT");
  runsl();
  delay(100);
  SIM.println("AT+HTTPPARA=CID,1");
  runsl();
  delay(100);
  SIM.println("AT+HTTPPARA=URL," + URL + ran);
  runsl();
  delay(100);
  SIM.println("AT+HTTPACTION=0");
  runsl();
  delay(5000);
  SIM.println("AT+HTTPREAD");
  runsl();
  delay(100);
  SIM.println("AT+HTTPTERM");
  runsl();
}

//Print GSM Status
void runsl() {
  while (SIM.available()) {
    Serial.write(SIM.read());
  }
}
