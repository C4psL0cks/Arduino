//#include <SoftwareSerial.h>
//
//SoftwareSerial mySerial(10, 11); // TX, RX
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(9600);
//  while(!Serial){
//    ;
//  }
//  mySerial.begin(9600);
//  Serial.println("Config....");
//}
//
//void loop() {
//  if (mySerial.available()) {
//    Serial.write(mySerial.read());
//  }
//  if (Serial.available()) {
//    mySerial.write(Serial.read());
//  }
//}

String responseString;
unsigned long previous=0;
unsigned long beltTime;
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
//  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
//  updateSerial();
//  mySerial.println("AT+CSTT=internet,\"true\",\"true\""); //Check whether it has registered in the network
//  updateSerial();
//  mySerial.println("AT+CSTT?"); //Check whether it has registered in the network
//  updateSerial();
}

void loop()
{
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
