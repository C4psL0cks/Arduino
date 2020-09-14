#include <LiquidCrystal_I2C.h>              // เรียกใช้งานไลบารีควบคุมหน้าจอ LCD ขนาด 16 * 4
LiquidCrystal_I2C lcd(0x27, 16, 2);         // กำหนด Address การเชื่อมต่อกับจอ LCD เป็น 0x27 ขนาดจอ 16 * 4 

#include <OneWire.h>                        // เรียกใช้ libary ของ เซ็นเซอร์ อุณหภูมิ
#include <DallasTemperature.h>              // เรียกใช้ libary ของ เซ็นเซอร์ อุณหภูมิ

#define TEMPS 8                             // กำหนดขา เซ็นเซอร์ อุณหภูมิ
#define BUZZER 7                            // กำหนดขา เซ็นเซอร์ ลำโพง
OneWire oneWire(TEMPS);                     // เรียกใช้ libary ของ เซ็นเซอร์ อุณหภูมิ
DallasTemperature sensors(&oneWire);        // เรียกใช้ libary ของ เซ็นเซอร์ อุณหภูมิ

void setup()                                // ฟั่งชัน setup มีไว้สำหรับการกำหนดค่าเริ่มต้นต่างๆก่อนเริ่มการทำงาน
{
  Serial.begin(115200);                     // กำหนดการส่งข้อมูลผ่านพอต ที่มีขนาด 115200
  // Start the DS18B20 sensor
  sensors.begin();                          // เรียกใช้ งาน เซ็นเซอร์ อุณหภูมิ
  pinMode(BUZZER, OUTPUT);                  // กำหนด ขา output ของ ลำโพง
  digitalWrite(BUZZER, HIGH);               // สั่งให้ลำโพงปิดเสียงก่อนตอนเริ่มต้น
  Serial.println();
  Serial.println("-------------------------------------");
  Serial.println("Running!");
  Serial.println("-------------------------------------");
  lcd.begin();                              // เริ่มการเชื่อมต่อจอ
  lcd.setCursor(0, 0);                      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 1
  lcd.print("$Hello, Welcome$");            // แสดงคำว่า $Hello, Welcome$
  delay(2000);                              // หน่วงเวลา
  lcd.clear();                              // เคลียร์จอ

}
void loop() // ฟังชั่น loop จะวนการทำงานไปเรื่อยๆ เหมื่อนการรันการทำงานตลอดเวลา อะไรที่อยู่ในนี้จะทำงานตลอดเวลา
{

  sensors.requestTemperatures();                                //  เรียกใช้ค่าจาก เซ็นเซอร์ อุณหภูมิ
  float Temperature = sensors.getTempCByIndex(0);               //   เรียกใช้ค่าจาก เซ็นเซอร์ อุณหภูมิ
  Serial.println("Temperature: " + String(Temperature) + "°C"); //แสดงคำว่า  Temp:  30  c

  if (Temperature >= 18 && Temperature <= 26) {                 //เช้ค ว่า อุณหภูมิ อยู่ในช่วง 18-26 
    Serial.println("not alarm!!!");                             //แสดงคำว่า  not alarm!!!
    lcd.setCursor(0, 1);                                        // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 0 แถวที่ 1
    lcd.print("not alarm!!!");                                  //แสดงคำว่า  not alarm!!!
    digitalWrite(BUZZER, HIGH);                                 // สั่งให้ลำโพงปิดเสียง
  }
  else {  
    Serial.println("alarm!!!");                                 //แสดงคำว่า  alarm!!!
    lcd.setCursor(0, 1);                                        // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 0 แถวที่ 1
    lcd.print("alarm!!!");                                      //แสดงคำว่า  alarm
    digitalWrite(BUZZER, LOW);                                  // สั่งให้ลำโพงเปิดเสียง
  }

  lcd.setCursor(0, 0);                                          // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 0 แถวที่ 0
  lcd.print("Temp: " + String(Temperature) + " C");             //แสดงคำว่า  Temp:  30  c
  delay(100);                                                   // หน่วงเวลา 0.1 วิ
}
