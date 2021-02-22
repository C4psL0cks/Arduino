#include <HardwareSerial.h>

#define TX0 1
#define RX0 3

#define TX1 10
#define RX1 9

#define TX2 17
#define RX2 16

#define simRXPIN  27
#define simTXPIN  26

//HardwareSerial UART0(0); // not rx,tx,3,1
//HardwareSerial UART0(1); // not rx,tx,9,10
HardwareSerial UART0(2); // ok rx,tx,16,17

void setup() {
  //  Serial.begin(9600);
  //  UART0.begin(9600, SERIAL_8N1, RX0, TX0); // tx,rx -> rx,tx
  //  Serial.begin(9600);
  //  UART0.begin(9600, SERIAL_8N1, RX1, TX1); // tx,rx -> rx,tx
  Serial.begin(9600);
  UART0.begin(9600, SERIAL_8N1, RX2, TX2); // tx,rx -> rx,tx
  //  Serial.begin(115200);
  //  UART0.begin(115200, SERIAL_8N1, simRXPIN, simTXPIN);
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
