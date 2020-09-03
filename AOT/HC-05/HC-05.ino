//HM-10 Setup
#include <AltSoftSerial.h>
AltSoftSerial bluetooth;

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 1
#define CMD_ANALOGWRITE 2
#define CMD_CONTROL 3
#define PIN_HIGH 1
#define PIN_LOW 0

char get_Star = ' ';
int Read_command = 0;
int pin_number_digital = 0;
int pin_number_analog = 0;
int pin_state = 0;
int pin_value = 0;
int pin_value_state = 0;


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

  if (Serial.available()) {           // Read serial
    int incomingByte = Serial.parseInt();
    Serial.println(incomingByte);
    //    bluetooth.write(Serial.read());   // bluetooth = Serial.read() data form blue send to
    bluetooth.write(incomingByte);
  }

  if (bluetooth.available()) {        // get blue to show in serialmonitor
    get_Star = bluetooth.read();
  }

  if (get_Star != START_CMD_CHAR) {
    return;
  }
  else {
    // parse incoming command type 1|
    Read_command = bluetooth.parseInt(); // read the command
    //Serial.println("Read_command:" + String(Read_command));

    // 1) GET digitalWrite DATA FROM ARDUDROID *1|
    if (Read_command == CMD_DIGITALWRITE) {
      Serial.println("CMD_DIGITALWRITE");
      // digital
      //*1|1|1# HIGH
      //*1|1|0# LOW

      // parse incoming pin# and value |1|
      pin_number_digital = bluetooth.parseInt(); // read the pin
      Serial.println("Pin_number_digital:" + String(pin_number_digital));

      // parse incoming pin# and value |3|#
      pin_state = bluetooth.parseInt(); // read the pin
      Serial.println("Pin_state:" + String(pin_state));

      if (pin_state == PIN_LOW) {
        Serial.println("Pin_state LOW:" + String(pin_state));
        pin_value_state = LOW;
      }
      if (pin_state == PIN_HIGH) {
        Serial.println("Pin_state HIGH:" + String(pin_state));
        pin_value_state = HIGH;
      }
      // send pin_number and pin_value to function
      return;
    }

    // 2) GET analogWrite DATA FROM ARDUDROID *2|
    if (Read_command == CMD_ANALOGWRITE) {
      Serial.println("CMD_ANALOGWRITE");

      // analog
      //*2|1|93#

      // parse incoming pin# and value |1|
      pin_number_analog = bluetooth.parseInt(); // read the pin
      Serial.println("Pin_number_analog:" + String(pin_number_analog));

      // parse incoming pin# and value |93|#
      pin_value = bluetooth.parseInt();  // read the value
      Serial.println("Pin_value:" + String(pin_value));

      //analogWrite( pin_num, pin_value );
      return;  // Done. return to loop();
    }

    // 1) GET digitalWrite DATA FROM ARDUDROID *1|
    if (Read_command == CMD_CONTROL) {
      Serial.println("CMD_CONTROL");
      // digital
      //*3|1|1# HIGH
      //*3|1|0# LOW

      // parse incoming pin# and value |1|
      pin_number_digital = bluetooth.parseInt(); // read the pin
      Serial.println("Pin_number_digital:" + String(pin_number_digital));

      // parse incoming pin# and value |3|#
      pin_state = bluetooth.parseInt(); // read the pin
      Serial.println("Pin_state:" + String(pin_state));

      if (pin_state == PIN_LOW) {
        Serial.println("Pin_state LOW:" + String(pin_state));
        pin_value_state = LOW;
      }
      if (pin_state == PIN_HIGH) {
        Serial.println("Pin_state HIGH:" + String(pin_state));
        pin_value_state = HIGH;
      }
      //send pin_number and pin_value to function
      return;
    }
  }

  //  if (bluetooth.available()) {
  //    // รับส่งแบบ decimal
  //    //    incomingByte = bluetooth.read();
  //    //    Serial.println(incomingByte, DEC);
  //
  //    // รับส่งแบบ ascii --->แอปส่งแบบ acsii  arduino รับแบบ ascii แล้ว write ออก
  //    Serial.write(bluetooth.read());
  //    //    int number = bluetooth.read();
  //    //    Serial.println(number);
  //
  //    // รับแบบข้อความ
  //    //    String msg = bluetooth.readString();
  //    //    Serial.println("Echo: " + msg);
  //
  //  }
  //
  //  if (Serial.available()) {
  //    // รับส่งแบบ decimal จากการพิมพ์ผ่านหน้าต่าง serial
  //
  //    // แบบรับข้อความแล้วแปลงเป็น ตัวเลข
  //    //    incomingByte = Serial.read();
  //    //    Serial.println(incomingByte, DEC);
  //
  //    // แบบรับตัวเลข
  //    incomingByte = Serial.parseInt();
  //    Serial.println(incomingByte);
  //
  //    // ส่งออกเป็น ascii ต้องส่งแบบ no line ending
  //    //    bluetooth.write(incomingByte);
  //
  //    // ส่งออกเป็น ascii ต้องส่งแบบปกติ ตัด ending
  //    if (incomingByte != 10 & incomingByte != 13 )
  //    {
  //      bluetooth.write(incomingByte);
  //    }
  //    //    bluetooth.write(0x45); // will write 0100 0101 to the cable
  //    //    bluetooth.write(0x48);   // H
  //    //    bluetooth.write(0x45);   // E
  //    //    bluetooth.write(0x4C);   // L
  //    //    bluetooth.write(0x4C);   // L
  //    //    bluetooth.write(0x4F);   // O
  //  }
  //  //   ส่งไปหาแอปตลอด ทุกๆ 1 วิ
  //  //    for (int i = 0; i <= 10; i++) {
  //  //      bluetooth.write(i);
  //  //      //    Serial.println(i);
  //  //      delay(1000);
  //  //    }
}