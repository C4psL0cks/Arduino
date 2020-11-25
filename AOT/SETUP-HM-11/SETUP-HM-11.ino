//HM-10 Setup
#include <AltSoftSerial.h>
AltSoftSerial bluetooth;

int incomingByte = 0; // for incoming serial data
void setup()
{
  Serial.begin(9600);  // Debug Serial
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.setTimeout(100);
  while (!Serial);
  bluetooth.begin(9600);  // to AltSoftSerial RX
  bluetooth.setTimeout(100);
  Serial.println("bluetooth AltSoftSerial started at 9600");
}
void loop()
{

  if (bluetooth.available()) {
    // รับส่งแบบ decimal
    //    incomingByte = bluetooth.read();
    //    Serial.println(incomingByte, DEC);

    // รับส่งแบบ ascii --->แอปส่งแบบ acsii  arduino รับแบบ ascii แล้ว write ออก
    //    Serial.write(bluetooth.read());
    int number = bluetooth.read();
    Serial.println(number);

    // รับแบบข้อความ
    //    String msg = bluetooth.readString();
    //    Serial.println("Echo: " + msg);

  }

  if (Serial.available()) {
    // รับส่งแบบ decimal จากการพิมพ์ผ่านหน้าต่าง serial

    // แบบรับข้อความแล้วแปลงเป็น ตัวเลข
    //    incomingByte = Serial.read();
    //    Serial.println(incomingByte, DEC);

    // แบบรับตัวเลข
    incomingByte = Serial.parseInt();
    Serial.println(incomingByte);

    // ส่งออกเป็น ascii ต้องส่งแบบ no line ending
    //    bluetooth.write(incomingByte);

    // ส่งออกเป็น ascii ต้องส่งแบบปกติ ตัด ending
    if (incomingByte != 10 & incomingByte != 13 )
    {
      bluetooth.write(incomingByte);
    }
    //    bluetooth.write(0x45); // will write 0100 0101 to the cable
    //    bluetooth.write(0x48);   // H
    //    bluetooth.write(0x45);   // E
    //    bluetooth.write(0x4C);   // L
    //    bluetooth.write(0x4C);   // L
    //    bluetooth.write(0x4F);   // O
  }
  //   ส่งไปหาแอปตลอด ทุกๆ 1 วิ
  //    for (int i = 0; i <= 10; i++) {
  //      bluetooth.write(i);
  //      //    Serial.println(i);
  //      delay(1000);
  //    }
}
