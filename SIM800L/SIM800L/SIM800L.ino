#include <SoftwareSerial.h>
static const int SimRXPin = 3, SimTXPin = 2;
SoftwareSerial SIM(SimTXPin, SimRXPin); // TX,RX

void setup() {

  Serial.begin(9600);
  SIM.begin(9600);
  Serial.println("Initializing...");
  delay(500);

  SIM.println("AT"); // Once the handshake test is successful, it will back to OK
  up();
  SIM.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  up();
  SIM.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  up();
  SIM.println("AT+CREG?"); //Check whether it has registered in the network
  up();
}

void loop() {
  up();
}
void up() {
  while (Serial.available()){
    SIM.write(Serial.read());
  }
  while (SIM.available()){
    Serial.write(SIM.read());
  }
}
