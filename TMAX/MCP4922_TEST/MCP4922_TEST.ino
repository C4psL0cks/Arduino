#include <MCP4922.h>
#include <SPI.h>
MCP4922 DAC(50, 52, 53, 48); // (MOSI,SCK,CS,LDAC) define Connections for UNO_board,
int number;
void setup() {
  Serial.begin(115200);
  SPI.begin();
  Serial.println("Start...Menu");
  Serial.println("===================");
  Serial.println("0. 0 V");
  Serial.println("1. 1 V");
  Serial.println("2. 2 V");
  Serial.println("3. 3 V");
  Serial.println("4. 4 V");
  Serial.println("5. 5 V");
  Serial.println("===================");
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  DAC.Set(0, 0); //1.0 v
  DAC.Set(2450, 0); //3.0 v
}
void loop() {

  if (Serial.available() > 0) {
    number = Serial.parseInt();
    Serial.println("Select :" + String(number));
    if (number == 1) {
      DAC.Set(830, 0); //1.0 v
      Serial.println("1. 1 V");
    }
    if (number == 2) {
      DAC.Set(1640, 0); //2.0 v
      Serial.println("2. 2 V");
    }
    if (number == 3) {
      DAC.Set(2450, 0); //3.0 v
      Serial.println("3. 3 V");
    }
    if (number == 4) {
      DAC.Set(3290, 0); //4.0 v
      Serial.println("4. 4 V");
    }
    if (number == 5) {
      DAC.Set(4095, 4095); //5.0 v
      Serial.println("5. 5 V");
    }
  }
}
