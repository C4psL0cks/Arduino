#include "max6675.h"
#include <RBDdimmer.h>

const int SCS1 = 10;
const int SCS2 = 11;
const int SSO = 12;
const int SSCK = 13;

const int Relay1 = 31;
const int Relay2 = 33;
const int Relay3 = 35;
const int Relay4 = 37;

dimmerLamp dimmer1(3);
dimmerLamp dimmer2(5);
dimmerLamp dimmer3(6);
MAX6675 S1(SSCK, SCS1, SSO);
MAX6675 S2(SSCK, SCS2, SSO);

int counter = 0;
int M1 = 0;
int M2 = 0;

void setup() {
  Serial.begin(115200);
  dimmer1.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE)
  dimmer2.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE)
  dimmer3.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE)
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  delay(500);
}

void loop() {


  dimmer1.setPower(M1); // name.setPower(0%-100%)
  dimmer2.setPower(M1); // name.setPower(0%-100%)
  dimmer3.setPower(M1); // name.setPower(0%-100%)

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
