#include <SPI.h>
#include <nRF24L01p.h>
nRF24L01p transmitter(7, 8); //CSN,CE
void setup() {
  delay(150);
  Serial.begin(115200);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
  transmitter.TXaddress("ALL"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  transmitter.init();
}
String message;
void loop() {
  int vr = analogRead(A0);
  Serial.println("VR:" + String(vr));
  transmitter.txPL(String(vr)); // ค่าที่ต้องการส่ง
  transmitter.send(FAST); // สั่งให้ส่งออกไป
  delay(200);
}
