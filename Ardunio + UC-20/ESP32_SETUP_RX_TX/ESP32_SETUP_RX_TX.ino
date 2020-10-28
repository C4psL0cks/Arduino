/*
  U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
  U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
  U2UXD is unused and can be used for your projects.
*/

#include <HardwareSerial.h>

#define TX0 1
#define RX0 3

#define TX1 10
#define RX1 9

#define TX2 17
#define RX2 16

#define simRXPIN  14
#define simTXPIN  12

//HardwareSerial UART0(0); // not rx,tx,3,1
//HardwareSerial UART0(1); // not rx,tx,9,10
HardwareSerial UART0(2); // ok rx,tx,16,17

void setup() {
  Serial.begin(9600);
  //UART0.begin(9600, SERIAL_8N1, RX2, TX2); // tx,rx -> rx,tx
  //  UART0.begin(9600, SERIAL_8N1, RX1, TX1); // tx,rx -> rx,tx
  //  UART0.begin(9600, SERIAL_8N1, RX2, TX2); // tx,rx -> rx,tx
  UART0.begin(9600, SERIAL_8N1, simRXPIN, simTXPIN);
  Serial.println("START.............");
  UART0.println("AT\r");
  wait_ok(1000);
}

void loop() {
  // read serial
  while (UART0.available()) {
    Serial.write(UART0.read());
  }
  // read monitor
  while (Serial.available()) {
    UART0.write(Serial.read());
  }
}

void wait_ok(long time) {
  while (UART0.available() > 0) {
    String req = UART0.readStringUntil('\n');
    Serial.println(req);
    if (req.indexOf(F("OK")) != -1) {
      Serial.println("OKKKK");
    }
  }
  delay(time);
}
