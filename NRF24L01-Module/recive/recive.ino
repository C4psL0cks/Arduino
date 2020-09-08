#include <SPI.h>
#include <nRF24L01p.h>
nRF24L01p receiver(7, 8); //CSN,CE

int output_pin_zf = 4;  //ZF - Direction pwm
int output_pin_vr = 5;  //VR - speed
int speeds;
int val;
String message;
int x;

void setup() {

  delay(150);
  Serial.begin(115200);

  pinMode(output_pin_zf, OUTPUT);
  pinMode(output_pin_vr, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
  receiver.RXaddress("ALL"); // ตั้งชื่อตำแห่นงให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  receiver.init();
  Serial.println("RUN...Recvicever");

}
void loop() {

  if (receiver.available()) {
    receiver.read(); // สั่งให้เริ่มอ่าน
    receiver.rxPL(x); // สั่งใหอ่านเก็บไว้ที่ตัวแปร
    Serial.println("VR: " + String(x));
    speeds = map(x, 0, 1023, 0, 255); // 0-255
    analogWrite(output_pin_vr, speeds); // Send PWM signal
    Serial.println("SPEED:" + String(speeds));
    message = "";
  }
  delay(10);

}
