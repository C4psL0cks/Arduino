#include "HX711.h"
#define calibration_factor -380000 // เอาค่าที่ได้จากการปรับ มาใส่ตรงนี้
#define DOUT  3
#define CLK  2
#define BUZZER 7
HX711 scale(DOUT, CLK);

float weight;
int number;
bool check1 = false;
bool check2 = false;
bool check3 = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);
  Serial.println("Calibrating...");
  scale.set_scale(calibration_factor); 
  scale.tare(); 
  Serial.println("Start...Menu");
  Serial.println("===================");
  Serial.println("1. 4 ล้อ");
  Serial.println("2. 6 ล้อ");
  Serial.println("3. 10 ล้อ");
  Serial.println("===================");
}
void loop() {

  weight =  scale.get_units();
  //Serial.println(String(weight) + " kg");

  if (Serial.available() > 0) {
    number = Serial.parseInt();
    Serial.println("Select :" + String(number));
    if (number == 1) {
      Serial.println("1. 4 ล้อ");
      check1 = true;
    }
    if (number == 2) {
      Serial.println("2. 6 ล้อ");
      check2 = true;
    }
    if (number == 3) {
      Serial.println("3. 10 ล้อ");
      check3 = true;
    }
    if (number == 4) {
      Serial.println("reset");
      check1 = false;
      check2 = false;
      check3 = false;
    }
    else {
      Serial.println("!!!!");
    }
  }

  if (check1) {
    if (weight >= 1.9) {
      BUZZER_ON();
      Serial.println("on");
    }
    else {
      BUZZER_OFF();
      Serial.println("off");
    }
  }
  if (check2) {
    if (weight >= 3.0) {
      BUZZER_ON();
      Serial.println("on");
    }
    else {
      BUZZER_OFF();
      Serial.println("off");
    }
  }
  if (check3) {
    if (weight > 4) {
      BUZZER_ON();
      Serial.println("on");
    }
    else {
      BUZZER_OFF();
      Serial.println("off");
    }
  }


}
void BUZZER_ON() {
  digitalWrite(BUZZER, LOW);
}
void BUZZER_OFF() {
  digitalWrite(BUZZER, HIGH);
}
