#include <SoftwareSerial.h>
SoftwareSerial portOne(2, 3);
SoftwareSerial portTwo(8, 9);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Start each software serial port
  portOne.begin(9600);
  portTwo.begin(9600);
}

void loop() {
  // By default, the last intialized port is listening.
  // when you want to listen on a port, explicitly select it:
  portOne.listen();
  while (Serial.available()) {
    portOne.write(Serial.read());
  }
  while (portOne.available()) {
    Serial.write(portOne.read());
  }

  //  portTwo.listen();
  //  while (Serial.available()) {
  //    portTwo.write(Serial.read());
  //  }
  //  while (portTwo.available()) {
  //    Serial.write(portTwo.read());
  //  }


}
