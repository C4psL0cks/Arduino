#include <LiquidCrystal_I2C.h> // เรียกใช้งานไลบารีควบคุมหน้าจอ LCD ขนาด 20 x 4
LiquidCrystal_I2C lcd(0x27, 20, 4); // กำหนด Address การเชื่อมต่อกับจอ LCD เป็น 0x27 ขนาดจอ 20 x 4 0x3F

#define IR_SW1 2  // กำหนด ขา sensor ir ตัวที่ 1 ขาเข้า บวกเลข
#define IR_SW2 3  // กำหนด ขา sensor ir ตัวที่ 2 ขาออก ลบเลข

/// sensor 1 Ultarsonic
const int trigPin1 = 4;  //กำหนด ขา sensor ขา 4
const int echoPin1 = 5;  //กำหนด ขา sensor ขา 5

/// sensor 2 Ultarsonic
const int trigPin2 = 6;  //กำหนด ขา sensor ขา 6
const int echoPin2 = 7;  //กำหนด ขา sensor ขา 7

/// sensor 3 Ultarsonic
const int trigPin3 = 8;  //กำหนด ขา sensor ขา 8
const int echoPin3 = 9;  //กำหนด ขา sensor ขา 9

int led1 = A0;           // กำหนด ขา led1 ขา A0
int led2 = A1;           // กำหนด ขา led2 ขา A1
int led3 = A2;           // กำหนด ขา led3 ขา A2
int state1 = 0, state2 = 0, state3 = 0; // กำหนด state การทำงานเอาไว้เช็คว่าถึง state ไหนแล้ว
int Ultrasonic1, Ultrasonic2, Ultrasonic3;

int counter1 = 0;                       // กำหนดตัวแปลสำหรับเอาไว้รอนับจำนวนรถเข้า - ออก
int counter2 = 3;                       // กำหนดตัวแปลสำหรับเอาไว้รอนับจำนวนที่ว่าง
boolean send_data1 = false;             //กำหนดการเช็คก่อนส่งค่า
boolean send_data2 = false;             //กำหนดการเช็คก่อนส่งค่า
unsigned long time_ms_buffer;           //กำหนดเวลาสำหรับการหน่วงเพื่อ + และ - ค่าใน ฟังชั่น

void IntSwitchplus(void) {                  //ฟังชั่น บวกเลข
  if ((millis() - time_ms_buffer) > 1000) { // รันเวลา เอามา - กับเวลาที่กำหนด ถ้าน้อยกว่า 1000 หรือ การหน่วงประมาณ 3 วินาที เมื่อเข้าเงื่อนไงนี้ให้ทำบรรทัดช้าล่างต่อ

    counter1++;                              //  counter1 ++ คือการ + 1 เพื่อบอกว่า รถเข้ามา 1
    counter2--;                              //  counter2 -- คือการ - 1 เพื่อบอกว่า ที่ว่างลดลง 1 เมื่อเข้ามา 1
    if (counter1 >= 4) {                     // เลข 10 เอาไว้ set จำนวนที่นั่ง ตอนปกติ คือ 0 ที่นั่ง เมื่อขึ้น จะ +1 จนครบ 10 จะเห็นว่า เมื่อไหร่ที่ค่า  counter มากว่า 10 หมายถึงเกิน 10 ให้ทำเงื่อนไขข้างล่างต่อ
      counter1 = 4;                          // คือการ set ค่าให้ counter นั่น เท่ากับ 10 ไปเลย
    }
    if (counter2 < 0) {                      // เอาไว้ set จำนวนที่ ตอนปกติ คือ 3 ที่ เมื่อลง จะ -1 ถ้าถึง 0 ที่นั่งให้เท่ากับ 0
      counter2 = 0;
    }
    Serial.println("Count1:" + String(counter1));
    Serial.println("Count2:" + String(counter2));
    send_data1 = true;                      // ถ้า + ค่าแล้วให้เปลี่ยนค่าจาก false เป็น true
  }
  time_ms_buffer = millis();                // เวลาที่กำหนด เท่ากับเวลาของการรัน
}

void IntSwitchwanting(void) {               //ฟังชั่น ลบเลข
  if ((millis() - time_ms_buffer) > 1000) { // รันเวลา เอามา - กับเวลาที่กำหนด ถ้าน้อยกว่า 1000 หรือ การหน่วงประมาณ 3 วินาที เมื่อเข้าเงื่อนไงนี้ให้ทำบรรทัดช้าล่างต่อ

    counter1--;                              //  counter1 -- คือการ - 1 เพื่อบอกว่า ที่ว่างเพิ่มขึ้น 1 เมื่อรถออก 1
    counter2++;                              //  counter2 ++ คือการ + 1 เพื่อบอกว่า รถไปออก 1
    if (counter1 < 0) {                      // เอาไว้ set จำนวนที่ ตอนปกติ คือ 3 ที่ เมื่อลง จะ -1 ถ้าถึง 0 ที่นั่งให้เท่ากับ 0
      counter1 = 0;
    }
    if (counter2 >= 3) {                     // เลข 10 เอาไว้ set จำนวนที่นั่ง ตอนปกติ คือ 0 ที่นั่ง เมื่อขึ้น จะ +1 จนครบ 10 จะเห็นว่า เมื่อไหร่ที่ค่า  counter มากว่า 10 หมายถึงเกิน 10 ให้ทำเงื่อนไขข้างล่างต่อ
      counter2 = 3;                          // คือการ set ค่าให้ counter นั่น เท่ากับ 10 ไปเลย
    }
    Serial.println("Count1:" + String(counter1));
    Serial.println("Count2:" + String(counter2));
    send_data2 = true;                       // ถ้า - ค่าแล้วให้เปลี่ยนค่าจาก false เป็น true
  }
  time_ms_buffer = millis();                 // เวลาที่กำหนด เท่ากับเวลาของการรัน
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
  pinMode(IR_SW1, INPUT);                  // กำหนดขาการเขื่อมต่อ ของ sensor ตัวที่ 1 เป็นแบบ input
  pinMode(IR_SW2, INPUT);                  // กำหนดขาการเขื่อมต่อ ของ sensor ตัวที่ 2 เป็นแบบ input

  attachInterrupt(digitalPinToInterrupt(IR_SW1), IntSwitchplus, FALLING);     //ขาขึ้น เมื่อมีสัญญานจากเซ็นเซอร์เข้ามา จำทำการทำงานทันที และเรียกฟังชั่น บวกเลขและเก็บค่าไว้ใน ตัวแปล counter
  attachInterrupt(digitalPinToInterrupt(IR_SW2), IntSwitchwanting, FALLING);  //ขาลง เมื่อมีสัญญานจากเซ็นเซอร์เข้ามา จำทำการทำงานทันที และเรียกฟังชั่น ลบเลขและเก็บค่าไว้ใน ตัวแปล counter

}
void loop() {                               // ฟังชั่น loop จะวนการทำงานไปเรื่อยๆ เหมื่อนการรันการทำงานตลอดเวลา อะไรที่อยู่ในนี้จะทำงานตลอดเวลา

  int Ultrasonic1 = sensor1(state1);
  int Ultrasonic2 = sensor2(state2);
  int Ultrasonic3 = sensor3(state3);

  lcd.setCursor(0, 0);                      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 0 แถวที่ 0
  lcd.print("======Welcome!======");        // เริ่มปริ้นคำว่า  Welcome! ตั้งแต่ ตำแหน่งที่ 0 ในแถวที่ 0 ตามที่กำหนด
  lcd.setCursor(1, 3);                      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 1 แถวที่ 3
  lcd.print("CAR : ");                      // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 1 ในแถวที่ 3 ตามที่กำหนด
  lcd.setCursor(8, 3);                      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 8 แถวที่ 3
  lcd.print(counter1);                      // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 8 ในแถวที่ 3 ตามที่กำหนด
  lcd.setCursor(10, 3);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 10 แถวที่ 3
  lcd.print("EMPTY: ");                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 10 ในแถวที่ 3 ตามที่กำหนด
  lcd.setCursor(18, 3);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 18 แถวที่ 3
  lcd.print(counter2);                      // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 18 ในแถวที่ 3 ตามที่กำหนด

  /// เช้คนับรถ
  if (send_data1 == true) {                  // เช้คว่า ถ้า ค่าของ send_data1 ถุกเปลี่ยนจาก false เป็น ture แสดงว่า ฟังชั่น + ทำงานแล้ว และ มีค่าเลขที่ถูกบวกแล้วอยู่ในตัวแปล  counter
    lcd.setCursor(1, 3);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 1 แถวที่ 3
    lcd.print("CAR : ");                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 1 ในแถวที่ 3 ตามที่กำหนด
    lcd.setCursor(8, 3);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 8 แถวที่ 3
    lcd.print(counter1);                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 8 ในแถวที่ 3 ตามที่กำหนด
    lcd.setCursor(10, 3);                    // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 10 แถวที่ 3
    lcd.print("EMPTY: ");                    // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 10 ในแถวที่ 3 ตามที่กำหนด
    lcd.setCursor(18, 3);                    // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 18 แถวที่ 3
    lcd.print(counter2);                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 18 ในแถวที่ 3 ตามที่กำหนด

  }
  if (send_data2 == true) {                  // เช้คว่า ถ้า ค่าของ send_data2 ถุกเปลี่ยนจาก false เป็น ture แสดงว่า ฟังชั่น - ทำงานแล้ว และ มีค่าเลขที่ถูกลบแล้วอยู่ในตัวแปล  counter
    lcd.setCursor(1, 3);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 1 แถวที่ 3
    lcd.print("CAR : ");                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 1 ในแถวที่ 3 ตามที่กำหนด
    lcd.setCursor(8, 3);                     // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 8 แถวที่ 3
    lcd.print(counter1);                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 8 ในแถวที่ 3 ตามที่กำหนด
    lcd.setCursor(10, 3);                    // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 10 แถวที่ 3
    lcd.print("EMPTY: ");                    // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 10 ในแถวที่ 3 ตามที่กำหนด
    lcd.setCursor(18, 3);                    // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 18 แถวที่ 3
    lcd.print(counter2);                     // เริ่มปริ้นค่าใน counter  ตั้งแต่ ตำแหน่งที่ 18 ในแถวที่ 3 ตามที่กำหนด
  }

  // เช็ครถจอด ช่องที่ 1
  if (Ultrasonic1 == 1) {
    digitalWrite(led1, LOW);                 // ส่งสัญญาน ไปที่ขา led1 แบบ low ทำให้ไฟดับ
    lcd.setCursor(1, 1);                       // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 1 แถว ที่ 1
    lcd.print("CHA 1: ");                    // แสดงค่า CHA 1:  ที่ตัวอักษรที่ 1 แถว ที่ 1
    lcd.setCursor(8, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 8 แถว ที่ 1
    lcd.print(Ultrasonic1);                  // แสดงค่า Ultrasonic1 ที่ตัวอักษรที่ 8 แถว ที่ 1
    delay(10);                               // หน่วงเวลา 0.1 วินาที
  }
  if (Ultrasonic1 == 0) {
    digitalWrite(led1, HIGH);                // ส่งสัญญาน ไปที่ขา led1 แบบ high ทำให้ไฟติด
    lcd.setCursor(1, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 1 แถว ที่ 1
    lcd.print("CHA 1: ");                    // แสดงค่า CHA 1:  ที่ตัวอักษรที่ 2 แถว ที่ 1
    lcd.setCursor(8, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 8 แถว ที่ 1
    lcd.print(Ultrasonic1);                  // แสดงค่า Ultrasonic1 ที่ตัวอักษรที่ 2 แถว ที่ 1
    delay(10);                               // หน่วงเวลา 0.1 วินาที
  }


  // เช็ครถจอด ช่องที่ 2
  if (Ultrasonic2 == 1) {
    digitalWrite(led2, LOW);                  // ส่งสัญญาน ไปที่ขา led2 แบบ low ทำให้ไฟดับ
    lcd.setCursor(10, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 10 แถว ที่ 1
    lcd.print("CHA 2: ");                     // แสดงค่า CHA 2:  ที่ตัวอักษรที่ 10 แถว ที่ 1
    lcd.setCursor(18, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 18 แถว ที่ 1
    lcd.print(Ultrasonic2);                   // แสดงค่า Ultrasonic2 ที่ตัวอักษรที่ 18 แถว ที่ 1
    delay(10);                                // หน่วงเวลา 0.1 วินาที

  }
  if (Ultrasonic2 == 0) {
    digitalWrite(led2, HIGH);                 // ส่งสัญญาน ไปที่ขา led2 แบบ high ทำให้ไฟติด
    lcd.setCursor(10, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 10 แถว ที่ 1
    lcd.print("CHA 2: ");                     // แสดงค่า CHA 2:  ที่ตัวอักษรที่ 10 แถว ที่ 1
    lcd.setCursor(18, 1);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 18 แถว ที่ 1
    lcd.print(Ultrasonic2);                   // แสดงค่า Ultrasonic2 ที่ตัวอักษรที่ 18 แถว ที่ 1
    delay(10);                                // หน่วงเวลา 0.1 วินาที
  }


  // เช็ครถจอด ช่องที่ 3
  if (Ultrasonic3 == 1) {
    digitalWrite(led3, LOW);                  // ส่งสัญญาน ไปที่ขา led3 แบบ low ทำให้ไฟดับ
    lcd.setCursor(6, 2);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 6 แถว ที่ 2
    lcd.print("CHA 3: ");                     // แสดงค่า CHA 1:  ที่ตัวอักษรที่ 6 แถว ที่ 2
    lcd.setCursor(13, 2);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 13 แถว ที่ 2
    lcd.print(Ultrasonic3);                   // แสดงค่า Ultrasonic1 ที่ตัวอักษรที่ 13 แถว ที่ 2
    delay(10);                                // หน่วงเวลา 0.1 วินาที
  }
  if (Ultrasonic3 == 0) {
    digitalWrite(led3, HIGH);                 // ส่งสัญญาน ไปที่ขา led3 แบบ high ทำให้ไฟติด
    lcd.setCursor(6, 2);                      // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 6 แถว ที่ 2
    lcd.print("CHA 3: ");                     // แสดงค่า CHA 1:  ที่ตัวอักษรที่ 6 แถว ที่ 2
    lcd.setCursor(13, 2);                     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 13 แถว ที่ 2
    lcd.print(Ultrasonic3);                   // แสดงค่า Ultrasonic1 ที่ตัวอักษรที่ 13 แถว ที่ 2
    delay(10);                                // หน่วงเวลา 0.1 วินาที
  }

}
int sensor1(int state1) {

  long duration1, cm1;                       // กำหนดตัวแปล หาค่าเฉลี่ย กำ ตัวแปล cm
  pinMode(led1, OUTPUT);                     // กำหนดการเชื่อมต่อของขา led แบบ output
  pinMode(trigPin1, OUTPUT);                 // กำหนดการเขื่อมต่อของขา sensor แบบ output
  digitalWrite(trigPin1, LOW);               // ส่งสัญญาน 0 ออกไปที่ขา sensor
  digitalWrite(trigPin1, HIGH);              // ส่งสัญญาน 1 ออกไปที่ขา sensor
  delayMicroseconds(5);                      // หน่วงเวลา 5 ไมโคเซค
  digitalWrite(trigPin1, LOW);               // ส่งสัญญาน 0 ออกไปที่ขา sensor
  pinMode(echoPin1, INPUT);                  // กำหนดการเขื่อมต่อของขา sensor แบบ imput
  duration1 = pulseIn(echoPin1, HIGH);       // นำตัวแปลหาค่าเฉลี่ยมา = ค่าของสัญญานขาเข้าที่ตอบกลับมา
  cm1 = duration1 / 29 / 2;                  // นำค่าเฉลี่ยที่ได้มาเข้าสูตรของ อัลตร้าโซนิก โดยนำค่าที่ได้จากการเฉลี่ยของ sensor มา หาร 29 และหาร 2 และจะได้ cm ออกมา
  if (cm1 < 8) {                             // จากนั้นเช้คว่า cm นั้นมีระยะห่าง น้อยกว่า 8 cm หรอไม่ ถ้าน้อยกว่าให้ทำงานที่อยู่ภายใต้เงื่อนไข
    return state1 = 1;                       // set ค่าตัวแปล ให้ เท่ากับ 1 หมายความว่า ช่อง 1 ไม่ว่าง
  } else {
    return state1 = 0;                       // set ค่าตัวแปล ให้ เท่ากับ 0 หมายความว่า ช่อง 1 ว่าง
  }
  delay(100);                                // หน่วงเวลา 0.1 วินาที
}
int sensor2(int state2) {

  long duration2, cm2;                      // กำหนดตัวแปล หาค่าเฉลี่ย กำ ตัวแปล cm
  pinMode(led2, OUTPUT);                    // กำหนดการเชื่อมต่อของขา led แบบ output
  pinMode(trigPin2, OUTPUT);                // กำหนดการเขื่อมต่อของขา sensor แบบ output
  digitalWrite(trigPin2, LOW);              // ส่งสัญญาน 0 ออกไปที่ขา sensor
  digitalWrite(trigPin2, HIGH);             // ส่งสัญญาน 1 ออกไปที่ขา sensor
  delayMicroseconds(5);                     // หน่วงเวลา 5 ไมโคเซค
  digitalWrite(trigPin2, LOW);              // ส่งสัญญาน 0 ออกไปที่ขา sensor
  pinMode(echoPin2, INPUT);                 // กำหนดการเขื่อมต่อของขา sensor แบบ imput
  duration2 = pulseIn(echoPin2, HIGH);      // นำตัวแปลหาค่าเฉลี่ยมา = ค่าของสัญญานขาเข้าที่ตอบกลับมา
  cm2 = duration2 / 29 / 2;                 // นำค่าเฉลี่ยที่ได้มาเข้าสูตรของ อัลตร้าโซนิก โดยนำค่าที่ได้จากการเฉลี่ยของ sensor มา หาร 29 และหาร 2 และจะได้ cm ออกมา
  if (cm2 < 8) {                            // จากนั้นเช้คว่า cm นั้นมีระยะห่าง น้อยกว่า 8 cm หรอไม่ ถ้าน้อยกว่าให้ทำงานที่อยู่ภายใต้เงื่อนไข
    return state2 = 1;                      // set ค่าตัวแปล ให้ เท่ากับ 1 หมายความว่า ช่อง 1 ไม่ว่าง
  } else {
    return state2 = 0;                      // set ค่าตัวแปล ให้ เท่ากับ 0 หมายความว่า ช่อง 1 ว่าง
  }
  delay(100);                               // หน่วงเวลา 0.1 วินาที
}
int sensor3(int state3) {

  long duration3, cm3;                       // กำหนดตัวแปล หาค่าเฉลี่ย กำ ตัวแปล cm
  pinMode(led3, OUTPUT);                     // กำหนดการเชื่อมต่อของขา led แบบ output
  pinMode(trigPin3, OUTPUT);                 // กำหนดการเขื่อมต่อของขา sensor แบบ output
  digitalWrite(trigPin3, LOW);               // ส่งสัญญาน 0 ออกไปที่ขา sensor
  digitalWrite(trigPin3, HIGH);              // ส่งสัญญาน 1 ออกไปที่ขา sensor
  delayMicroseconds(5);                      // หน่วงเวลา 5 ไมโคเซค
  digitalWrite(trigPin3, LOW);               // ส่งสัญญาน 0 ออกไปที่ขา sensor
  pinMode(echoPin3, INPUT);                  // กำหนดการเขื่อมต่อของขา sensor แบบ imput
  duration3 = pulseIn(echoPin3, HIGH);       // นำตัวแปลหาค่าเฉลี่ยมา = ค่าของสัญญานขาเข้าที่ตอบกลับมา
  cm3 = duration3 / 29 / 2;                  // นำค่าเฉลี่ยที่ได้มาเข้าสูตรของ อัลตร้าโซนิก โดยนำค่าที่ได้จากการเฉลี่ยของ sensor มา หาร 29 และหาร 2 และจะได้ cm ออกมา
  if (cm3 < 8) {                             // จากนั้นเช้คว่า cm นั้นมีระยะห่าง น้อยกว่า 8 cm หรอไม่ ถ้าน้อยกว่าให้ทำงานที่อยู่ภายใต้เงื่อนไข
    return state3 = 1;                       // set ค่าตัวแปล ให้ เท่ากับ 1 หมายความว่า ช่อง 1 ไม่ว่าง
  } else {
    return state3 = 0;                       // set ค่าตัวแปล ให้ เท่ากับ 0 หมายความว่า ช่อง 1 ว่าง
  }
  delay(100);                                // หน่วงเวลา 0.1 วินาที
}
