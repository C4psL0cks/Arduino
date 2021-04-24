//HM-10 Setup
#include <AltSoftSerial.h>
AltSoftSerial bluetooth; // rx bt--tx46,tx bt --rx48

void setup() {
  Serial.begin(9600);  // Debug Serial
  while (!Serial);
  bluetooth.begin(9600);   // to AltSoftSerial RX

  // connect to the remote Bluetooth module
  bluetooth.println("AT+IMME1");
  delay(1000);
  bluetooth.println("AT+ROLE1");
  delay(1000);
  bluetooth.println("AT+CONA1893D74545F5");
  delay(1000);

  Serial.println("AltSoftSerial");
}

void loop() {

  // Read from the Serial Monitor and send to the Bluetooth module
  //  if (Serial.available()) {
  //    bluetooth.write(Serial.read());
  //  }
  //  Read from the Bluetooth module and send to the Arduino Serial Monitor
  //  if (bluetooth.available()) {
  //    Serial.write(bluetooth.read());
  //  }
  Serial.flush();
  String text = String(1) + '_'
                + String(0) + '_'
                + String(1) + '_'
                + String(0) + '_'
                + String(1) + '_'
                + String(0) + '_'
                + String(1) + '_'
                + String(0) + '_'
                + String(1) + '_'
                + String(0) + '#';
  //  Serial.println(text);
  bluetooth.println("10");
  bluetooth.flush();
}
