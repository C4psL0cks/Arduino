#include <LiquidCrystal_I2C.h> // เรียกใช้งานไลบารีควบคุมหน้าจอ LCD ขนาด 20 * 4
LiquidCrystal_I2C lcd(0x27, 20, 4); // กำหนด Address การเชื่อมต่อกับจอ LCD เป็น 0x27 ขนาดจอ 20 * 4

#define IR_SW1 2  // กำหนด ขา sensor ตัวที่ 1 ขาขึ้น บวกเลข
#define IR_SW2 3  // กำหนด ขา sensor ตัวที่ 2 ขาลง ลบเลข

int counter = 0;  // กำหนดตัวแปลสำหรับเอาไว้รอนับจำนวน
boolean send_data1 = false; //กำหนดการเช็คก่อนส่งค่า 
boolean send_data2 = false; //กำหนดการเช็คก่อนส่งค่า
unsigned long time_ms_buffer; //กำหนดเวลาสำหรับการหน่วงเพื่อ + และ - ค่าใน ฟังชั่น

void IntSwitchplus(void) { //ฟังชั่น บวกเลข
  if ((millis() - time_ms_buffer) > 1000) { // รันเวลา เอามา - กับเวลาที่กำหนด ถ้าน้อยกว่า 1000 หรือ การหน่วงประมาณ 3 วินาที เมื่อเข้าเงื่อนไงนี้ให้ทำบรรทัดช้าล่างต่อ

    counter++;                              // + 1 ของจำนวนที่มี เช่น counter = 0 พอ เข้าเงื่อนไขก็ +1 จะได้ counter = 2
    
    if (counter >= 10) {                    // เลข 10 เอาไว้ set จำนวนที่นั่ง ตอนปกติ คือ 0 ที่นั่ง เมื่อขึ้น จะ +1 จนครบ 10 จะเห็นว่า เมื่อไหร่ที่ค่า  counter มากว่า 10 หมายถึงเกิน 10 ให้ทำเงื่อนไขข้างล่างต่อ
      counter = 10;                         // คือการ set ค่าให้ counter นั่น เท่ากับ 10 ไปเลย
    }
    send_data1 = true;                      // ถ้า + ค่าแล้วให้เปลี่ยนค่าจาก false เป็น true
  }
  time_ms_buffer = millis();                // เวลาที่กำหนด เท่ากับเวลาของการรัน
}
void IntSwitchwanting(void) { //ฟังชั่น ลบเลข
  if ((millis() - time_ms_buffer) > 1000) { // รันเวลา เอามา - กับเวลาที่กำหนด ถ้าน้อยกว่า 1000 หรือ การหน่วงประมาณ 3 วินาที เมื่อเข้าเงื่อนไงนี้ให้ทำบรรทัดช้าล่างต่อ

    counter--;                              // - 1 ของจำนวนที่มี เช่น counter = 10 พอ เข้าเงื่อนไขก็ -1 จะได้ counter = 9
    if (counter < 0) {                      // เอาไว้ set จำนวนที่นั่ง ตอนปกติ คือ 10 ที่นั่ง เมื่อลง จะ -1 ถ้าถึง 0 ที่นั่งให้เท่ากับ 0
      counter = 0;
    }
    send_data2 = true;                      // ถ้า - ค่าแล้วให้เปลี่ยนค่าจาก false เป็น true
  }
  time_ms_buffer = millis();                // เวลาที่กำหนด เท่ากับเวลาของการรัน
}

void setup()                               // ฟั่งชัน setup มีไว้สำหรับการกำหนดค่าเริ่มต้นต่างๆก่อนเริ่มการทำงาน
{
  Serial.begin(115200);                    // กำหนดการส่งข้อมูลผ่านพอต ที่มีขนาด 115200
  lcd.begin();                             // เริ่มการเชื่อมต่อจอ
  lcd.backlight();                         // set ให้สว่าง
  lcd.setCursor(2, 1);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 1
  lcd.print("Hello, Welcome!");            // เริ่มปริ้นคำว่า Hello, Welcome! ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 1 ตามที่กำหนด
  delay(2000);                             // หน่วงเวลา
  lcd.clear();                             // เคลียร์จอ
  lcd.setCursor(2, 1);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 1
  lcd.print(">>COUNT : ");                 // เริ่มปริ้นคำว่า >>COUNT :  ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 1 ตามที่กำหนด
  lcd.setCursor(12, 1);                    // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 12 แถวที่ 1
  lcd.print(counter);                      // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 2 ตามที่กำหนด
  lcd.setCursor(2, 2);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 2
  lcd.print(">>COUNT : ");                 // เริ่มปริ้นคำว่า >>COUNT :  ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 2 ตามที่กำหนด
  lcd.setCursor(12, 2);                    // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 12 แถวที่ 2
  lcd.print(counter);                      // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 2 ตามที่กำหนด
  pinMode(IR_SW1, INPUT);                  // กำหนดขาการเขื่อมต่อ ของ sensor ตัวที่ 1 เป็นแบบ input
  pinMode(IR_SW2, INPUT);                  // กำหนดขาการเขื่อมต่อ ของ sensor ตัวที่ 2 เป็นแบบ input
  
  attachInterrupt(digitalPinToInterrupt(IR_SW1), IntSwitchplus, FALLING);     //ขาขึ้น เมื่อมีสัญญานจากเซ็นเซอร์เข้ามา จำทำการทำงานทันที และเรียกฟังชั่น บวกเลขและเก็บค่าไว้ใน ตัวแปล counter
  attachInterrupt(digitalPinToInterrupt(IR_SW2), IntSwitchwanting, FALLING);  //ขาลง เมื่อมีสัญญานจากเซ็นเซอร์เข้ามา จำทำการทำงานทันที และเรียกฟังชั่น ลบเลขและเก็บค่าไว้ใน ตัวแปล counter

}
void loop() // ฟังชั่น loop จะวนการทำงานไปเรื่อยๆ เหมื่อนการรันการทำงานตลอดเวลา อะไรที่อยู่ในนี้จะทำงานตลอดเวลา
{
  if (send_data1 == true) {   // เช้คว่า ถ้า ค่าของ send_data1 ถุกเปลี่ยนจาก false เป็น ture แสดงว่า ฟังชั่น + ทำงานแล้ว และ มีค่าเลขที่ถูกบวกแล้วอยู่ในตัวแปล  counter 
    Serial.println(counter);  // แสดงค่า ที่อยู่ใน counter
    lcd.setCursor(2, 1);      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 1
    lcd.print(">>COUNT : ");  // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 1 ตามที่กำหนด
    lcd.setCursor(12, 1);     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 12 แถวที่ 1
    lcd.print(counter);       // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 12 ในแถวที่ 1 ตามที่กำหนด
  }
  if (send_data2 == true) {   // เช้คว่า ถ้า ค่าของ send_data2 ถุกเปลี่ยนจาก false เป็น ture แสดงว่า ฟังชั่น - ทำงานแล้ว และ มีค่าเลขที่ถูกลบแล้วอยู่ในตัวแปล  counter 
    Serial.println(counter);  // แสดงค่า ที่อยู่ใน counter
    lcd.setCursor(2, 2);      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 2
    lcd.print(">>COUNT : ");  // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 2 ตามที่กำหนด
    lcd.setCursor(12, 2);     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 12 แถวที่ 2
    lcd.print(counter);       // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 12 ในแถวที่ 2 ตามที่กำหนด
  }
}
