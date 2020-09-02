#include <SoftwareSerial.h>
SoftwareSerial gsm(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  gsm.begin(9600);
  Serial.println("Testing GSM SIM900L");
  delay(500);

}
 
void loop() {
  if (gsm.available()) {
    Serial.write(gsm.read());
  }
  if (Serial.available()) {
    gsm.write(Serial.read());
  }
}