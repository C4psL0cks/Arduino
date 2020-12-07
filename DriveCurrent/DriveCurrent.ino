#include <SoftwareSerial.h>
SoftwareSerial bluetooth(9, 8);
uint8_t events[] = {0, 0};

double amps = 0.0;
float vin = 0.0;

void setup() {

  Serial.begin(9600);
  while (!Serial);
  bluetooth.begin(9600);
  bluetooth.println("Hello World");
  Serial.println("\nConnected ...version 1.0.0");
}

void loop() {


  vin = ((analogRead(A0) * 5.17) / 1024.0) / (2200.0 / (4700.0 + 2200.0));
  Serial.println("Voltage V: " + String(vin, 2));

  if (vin < 0) {
    vin  = 0;
  }

  amps = ((((analogRead(A1) / 1023.0) * 5000) - 2500) / 66);

  if (amps < 0) {
    amps = 0;
  }

  Serial.println("Amps A :" + String(amps, 2));

  events[0] = vin;
  events[1] = amps;
  bluetooth.write(events[0]);
  bluetooth.write(events[1]);
  delay(500);

}
