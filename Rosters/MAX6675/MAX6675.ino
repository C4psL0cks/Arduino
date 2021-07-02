#include "max6675.h"

int SSO = 11;
int SCS1 = 12;
int SCS2 = 10;
int SSCK = 13;

MAX6675 S1(SSCK, SCS1, SSO);
MAX6675 S2(SSCK, SCS2, SSO);

void setup() {
  Serial.begin(115200);
  Serial.println("MAX6675");
  delay(500);
}

void loop() {

  Serial.println("TEMPS1 :" + String(S1.readCelsius()) + " C");
  Serial.println("TEMPS1 :" + String(S1.readFahrenheit()) + " F");
  Serial.println("*****************************************");
  Serial.println("TEMPS2 :" + String(S2.readCelsius()) + " C");
  Serial.println("TEMPS2 :" + String(S2.readFahrenheit()) + " F");
  Serial.println("*****************************************");
  delay(1000);
}
