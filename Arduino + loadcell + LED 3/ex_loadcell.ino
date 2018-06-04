#include "HX711.h" // libary ที่เก็บฟังชั่นในการใช้งานและคำนวนของ Divces

float calibration_factor = 91524.00;  //กำนดค่า calibration_factor ที่ได้มาจากการ calibration
#define zero_factor 8281858  //กำนดค่า zero_factor ที่ได้มาจากการ calibration

#define DOUT  A1  // กำหนดค่าขอสัญญานขาเข้าของ อุปกรณ์ เข้าที่ขา A1 เป็นขาสัญญานแบบ analog
#define CLK   A0  // กำหนดค่าขอสัญญานขาเข้าของ อุปกรณ์ เข้าที่ขา A0 เป็นขาสัญญานแบบ analog


int ledPin1 = 2; // Red
int ledPin2 = 3; // white
int ledPin3 = 4; // 7 LED

float offset = 0;  // กำหนดค่าตัวแปร offset = 0 เพื่อนำไป + กับฟังชี่น get_units_kg();
float get_units_kg(); // กำหนดค่าที่ออกจากฟังชี่น get_units_kg(); เป็น ทศนิยม
float data = 0; //กำหนด ค่าตัวแปล data สำหรับเก็บค่า น้ำหนักที่ออกมา

HX711 scale(DOUT, CLK); // ส่งค่าขาสัญญาณทั้ง 2 ค่าเข้า libary HX711.h

void setup()
{
  Serial.begin(115200); // กำหนดค่า port การเชื่อมค่อในการส่งข้อมูล เป้น buad rate 115200
  pinMode(ledPin1, OUTPUT); // กำหนดค่า output LED 1
  pinMode(ledPin2, OUTPUT); // กำหนดค่า output LED 2
  pinMode(ledPin3, OUTPUT); // กำหนดค่า output LED 3
  Serial.println("Load Cell");
  scale.set_scale(calibration_factor);  // ส่งค่าเข้าสูตร ใน libary HX711.h
  scale.set_offset(zero_factor); // ส่งค่าเข้าสูตร ใน libary HX711.h
}
void loop()
{
  Serial.print("Reading: ");
  float data = get_units_kg() + offset; // เรียก ฟังชั้น get_units_kg() มา+ กับ ค่า offset
  Serial.print(data);
  Serial.println(" kg");
  if (data <= 3.0)   // เช็คค่า ว่าน้ำหนักน้อยกว่า 3 กิโลหรือไม่
  {
    Serial.println("3 KG");
    ledon1();  // เปิด LED 1
    ledoff2(); // ปิด LED 2
    ledoff3(); // ปิด LED 3
  }
  else if (data > 3.0 && data < 5.0) // เช็คค่า ว่าน้ำหนักมากกว่า 3 หรือน้อยกว่า  5  กิโลหรือไม่
  {
    Serial.println("3 - 5 KG");
    ledon2(); // เปิด LED 2
    ledoff1(); // ปิด LED 1
    ledoff3(); // ปิด LED 3
  }
  else if (data >= 5.0) // เช็คค่า ว่าน้ำหนักมากว่า 5  กิโลหรือไม่
  {
    Serial.println("5+ KG");
    ledon3(); // เปิด LED 3
    ledoff1(); // ปิด LED 1
    ledoff2(); // ปิด LED 2
  }
}
float get_units_kg()  // ฟังชั่น น้ำหนัก
{
  return (scale.get_units() * 0.453592); //scale.get_units() นำน้ำหนักมาเข้าสูตรของ loadcell แล้ว รีเทินค่าที่ได้
}
void ledon1()// ฟังชั่นเปิด LED 1
{
  digitalWrite(ledPin1, HIGH); // เปิด LED 1
  delay(100);
}
void ledoff1() // ฟังชั่นปิด LED 1
{
  digitalWrite(ledPin1, LOW); // ปิด LED 1
  delay(100); // หน่วงเวลา 0.1 วินาที 1000 เท่ากัย 1 วินาที
}
void ledon2() // ฟังชั่นเปิด LED 2
{
  digitalWrite(ledPin2, HIGH); // เปิด LED 2
  delay(100); // หน่วงเวลา 0.1 วินาที

}
void ledoff2() // ฟังชั่นเปิด LED 2
{
  digitalWrite(ledPin2, LOW); // ปิด LED 2
  delay(100); // หน่วงเวลา 0.1 วินาที
}
void ledon3() // ฟังชั่นเปิด LED 3
{
  digitalWrite(ledPin3, HIGH); // เปิด LED 3
  delay(100); // หน่วงเวลา 0.1 วินาที

}
void ledoff3() // ฟังชั่นปิด LED 3
{
  digitalWrite(ledPin3, LOW); // ปิด LED 3
  delay(100); // หน่วงเวลา 0.1 วินาที
}
