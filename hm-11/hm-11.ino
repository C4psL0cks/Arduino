////HM-10 Setup
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(8, 9);
//void setup()
//{
//  mySerial.begin(9600);
//  Serial.begin(9600);
//  delay(100);
//}
//void loop()
//{
// Read from the Serial Monitor and send to the Bluetooth module
//  if (Serial.available() > 0)
//    mySerial.write(Serial.read());
// Read from the Bluetooth module and send to the Arduino Serial Monitor
//  if (mySerial.available() > 0)
//    Serial.write(mySerial.read());
//}

//HM-10 Setup
#include <AltSoftSerial.h>
AltSoftSerial altser;

int incomingcase;
void setup() {
  Serial.begin(9600);  // Debug Serial
  Serial.setTimeout(100);
  altser.begin(9600);   // to AltSoftSerial RX
  altser.setTimeout(100);
  Serial.println("AltSoftSerial");
}

void loop() {

  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available()) {
    String msg = Serial.readString();
    Serial.println("Echo: " + msg);
    altser.print(msg);
  }

  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (altser.available()) {
    incomingcase = altser.read();
    Serial.println("APP TO BT:" + String(incomingcase));
  }
}
