#include <SoftwareSerial.h>
static const int SimRXPin = 3, SimTXPin = 2;
SoftwareSerial SIM(SimTXPin, SimRXPin); // TX,RX

void setup() {
  Serial.begin(9600);
  SIM.begin(9600);
  Serial.println("Initializing...");
  delay(500);
}

void loop() {
  if (Serial.available()){
    SIM.write(Serial.read());
  }
  if (SIM.available()){
    Serial.write(SIM.read());
  }
}
