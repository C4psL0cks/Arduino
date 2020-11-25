//HM-10 Setup
#include <AltSoftSerial.h>
AltSoftSerial altser;

void setup() {
  Serial.begin(9600);  // Debug Serial
  altser.begin(9600);   // to AltSoftSerial RX
  Serial.println("AltSoftSerial");
  delay(100);
}

void loop() {

  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available() > 0) {
    altser.write(Serial.read());
  }

  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (altser.available() > 0) {
    Serial.write(altser.read());
  }
}
