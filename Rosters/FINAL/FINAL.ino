#include "max6675.h"

const int CS1 = 2;
const int INC1 = 3;
const int UD1 = 4;
const int CS2 = 7;
const int INC2 = 8;
const int UD2 = 9;
const int SSO = 11;
const int SCS1 = 12;
const int SCS2 = 10;
const int SSCK = 13;
const int Relay1 = 31;
const int Relay2 = 33;
const int Relay3 = 35;
const int Relay4 = 37;

MAX6675 S1(SSCK, SCS1, SSO);
MAX6675 S2(SSCK, SCS2, SSO);

int counter = 0;
int M1 = 0;
int M2 = 0;

void setup() {
  Serial.begin(115200);

  pinMode(CS1, OUTPUT);
  pinMode(INC1, OUTPUT);
  pinMode(UD1, OUTPUT);
  pinMode(CS2, OUTPUT);
  pinMode(INC2, OUTPUT);
  pinMode(UD2, OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  digitalWrite(CS1, HIGH);
  digitalWrite(INC1, HIGH);
  digitalWrite(UD1, HIGH);
  digitalWrite(CS2, HIGH);
  digitalWrite(INC2, HIGH);
  digitalWrite(UD2, HIGH);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  delay(500);
}

void loop() {
  if (Serial.available()) {
    int incomingcase = Serial.parseInt();
    Serial.println(incomingcase);
    switch (incomingcase) {
      case 0:
        counter = 0;
        break;
      case 1:
        counter = 1;
        break;
      case 2:
        counter = 2;
        break;
      case 3:
        counter = 3;
        break;
      case 4:
        counter = 4;
        break;
      case 5:
        counter = 5;
        break;
      case 60:
        digitalWrite(Relay1, LOW);
        break;
      case 61:
        digitalWrite(Relay1, HIGH);
        break;

      case 70:
        digitalWrite(Relay2, LOW);
        break;
      case 71:
        digitalWrite(Relay2, HIGH);
        break;

      case 80:
        digitalWrite(Relay3, LOW);
        break;
      case 81:
        digitalWrite(Relay3, HIGH);
        break;

      case 90:
        digitalWrite(Relay4, LOW);
        break;
      case 91:
        digitalWrite(Relay4, HIGH);
        break;
    }
  }

  if (counter == 0) {
    M1 = 0;
    M2 = 0;
    Motor1(M1);
    Motor2(M2);
  }

  if (counter == 1) {
    M1 = 20;
    M2 = 20;
    Motor1(M1);
    Motor2(M2);
  }

  if (counter == 2) {
    M1 = 40;
    M2 = 40;
    Motor1(M1);
    Motor2(M2);
  }

  if (counter == 3) {
    M1 = 60;
    M2 = 60;
    Motor1(M1);
    Motor2(M2);
  }

  if (counter == 4) {
    M1 = 80;
    M2 = 80;
    Motor1(M1);
    Motor2(M2);
  }

  if (counter == 5) {
    M1 = 100;
    M2 = 100;
    Motor1(M1);
    Motor2(M2);
  }

  Serial.println("TEMPS1 :" + String(S1.readCelsius()) + " C");
  Serial.println("TEMPS1 :" + String(S1.readFahrenheit()) + " F");
  Serial.println("*****************************************");
  Serial.println("TEMPS2 :" + String(S2.readCelsius()) + " C");
  Serial.println("TEMPS2 :" + String(S2.readFahrenheit()) + " F");
  Serial.println("*****************************************");
  Serial.println("P1:" + String(M1));
  Serial.println("P2:" + String(M2));
  Serial.println("*****************************************");
  delay(1000);
}


void Motor1(int percent) {
  digitalWrite(UD1, LOW);
  digitalWrite(CS1, LOW);
  for (int i = 0; i < 100; i++) {
    digitalWrite(INC1, LOW);
    delayMicroseconds(1);
    digitalWrite(INC1, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(UD1, HIGH);
  for (int i = 0; i < percent; i++) {
    digitalWrite(INC1, LOW);
    delayMicroseconds(1);
    digitalWrite(INC1, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(CS1, HIGH);
}

void Motor2(int percent) {
  digitalWrite(UD2, LOW);
  digitalWrite(CS2, LOW);
  for (int i = 0; i < 100; i++) {
    digitalWrite(INC2, LOW);
    delayMicroseconds(1);
    digitalWrite(INC2, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(UD2, HIGH);
  for (int i = 0; i < percent; i++) {
    digitalWrite(INC2, LOW);
    delayMicroseconds(1);
    digitalWrite(INC2, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(CS2, HIGH);
}
