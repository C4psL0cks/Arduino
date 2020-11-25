//HM-10 Setup
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 8);

void setup() {
  
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("AltSoftSerial");
  delay(100);
}
void loop()
{
  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available() > 0)
    mySerial.write(Serial.read());

  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (mySerial.available() > 0)
    Serial.write(mySerial.read());
}
