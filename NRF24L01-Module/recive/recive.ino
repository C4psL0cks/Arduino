#include <SPI.h>
#include <nRF24L01p.h>
nRF24L01p receiver(7, 8); //CSN,CE

int output_pin_zf = 4;  //ZF - Direction pwm
int output_pin_vr = 5;  //VR - speed
int speeds;
int val;
int vr;
int state;

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
    receiver.read(); 
    receiver.rxPL(vr); 
    receiver.rxPL(state);
    Serial.println("VR: " + String(vr));
    Serial.println("STATE: " + String(state));
    speeds = map(vr, 20, 800, 0, 255); // 0-255
    analogWrite(output_pin_vr, speeds); // Send PWM signal
    Serial.println("SPEED:" + String(speeds));
  }
}
