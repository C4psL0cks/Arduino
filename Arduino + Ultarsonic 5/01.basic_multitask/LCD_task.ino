// task สำหรับจอ lcd 

#include <LiquidCrystal_I2C.h>          // เรียกใช้ไลบารีควบคุมจอ LCD

void LCD_func(void*) {                  // ฟังชั่น จอ lcd

  LiquidCrystal_I2C lcd(0x3F, 20, 4); // กำหนดค่า address  การเชื่อมต่อที่ address 0x3f ขนาดจอ 20 x 4 20 ตัวอักษร 4 แถว

  VOID SETUP() {                      // ฟังชั่นย่อย setup มีหน้าที่กำหนดค่าเริ่มต้นต่างๆก่อนเริ่มการทำงาน
    lcd.begin();
    lcd.backlight();                 // set จอให้สว่าง
    lcd.setCursor(2, 1);             // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 2 แถว ที่ 1
    lcd.print("Hello, Welcome!");   // แสดงค่า Hello, Welcome! ที่ตัวอักษรที่ 2 แถว ที่ 1
    DELAY(2000);                    // หน่วงเวลา 2 วินาที่ 1000 = 1 วินาที
    lcd.clear();
  }
  VOID LOOP() {                   // ฟังชั่น loop ย่อย มีหน้าที่รันคำสั่งแบบเป็น loop ไปเรื่อยๆ
    
    lcd.setCursor(2, 0);          // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 2 แถว ที่ 0
    lcd.print("Hello, Welcome!"); // แสดงค่า Hello, Welcome! ที่ตัวอักษรที่ 2 แถว ที่ 0

    ///////  ล้อคที่ 1
    if (checkstatus1 == 1) {   // เช้คค่า ถ้าตัวแปล checkstatus1 = 1 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(1, 1);     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 2 แถว ที่ 0
      lcd.print("LOG 1:");     // แสดงค่า Hello, Welcome! ที่ตัวอักษรที่ 2 แถว ที่ 0
      lcd.setCursor(8, 1);     // กำหนดเคอเซอร์ ให้เริ่มตั้งที่ตัวอักษรที่ 2 แถว ที่ 0
      lcd.print(checkstatus1);          // แสดงค่า Hello, Welcome! ที่ตัวอักษรที่ 2 แถว ที่ 0
      delay(10);               // หน่วงเวลา 0.1 วินาที
    }

    //// จากคอมเมนต์ด้านบน  ที่เหลือทำเหมือนด้านบน แค่ขับตำแหน่งการแสดงตัวอักษรและ แถว
    
    else if (checkstatus1 == 0) {  // เช้คค่า ถ้าตัวแปล checkstatus1 = 0 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(1, 1);
      lcd.print("LOG 1:");
      lcd.setCursor(8, 1);
      lcd.print(checkstatus1);
      delay(10);
    }

    ///////  ล้อคที่ 2 
    if (checkstatus2 == 1) {  // เช้คค่า ถ้าตัวแปล checkstatus2 = 1 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(1, 2);
      lcd.print("LOG 2:");
      lcd.setCursor(8, 2);
      lcd.print(checkstatus2);
      delay(10);
    }
    else if (checkstatus2 == 0) {  // เช้คค่า ถ้าตัวแปล checkstatus2 = 0 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(1, 2);
      lcd.print("LOG 2:");
      lcd.setCursor(8, 2);
      lcd.print(checkstatus2);
      delay(10);
    }

    ///////  ล้อคที่ 3
    if (checkstatus3 == 1) {  // เช้คค่า ถ้าตัวแปล checkstatus3 = 1 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(10, 1);
      lcd.print("LOG 3:");
      lcd.setCursor(18, 1);
      lcd.print(checkstatus3);
      delay(10);
    }
    else if (checkstatus3 == 0) {  // เช้คค่า ถ้าตัวแปล checkstatus3 = 0 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(10, 1);
      lcd.print("LOG 3:");
      lcd.setCursor(18, 1);
      lcd.print(checkstatus3);
      delay(10);
    }


    ///////  ล้อคที่ 4
    if (checkstatus4 == 1) {  // เช้คค่า ถ้าตัวแปล checkstatus4 = 1 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(10, 2);
      lcd.print("LOG 4:");
      lcd.setCursor(18, 2);
      lcd.print(checkstatus4);
      delay(10);
    }
    else if (checkstatus4 == 0) {  // เช้คค่า ถ้าตัวแปล checkstatus4 = 0 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(10, 2);
      lcd.print("LOG 4:");
      lcd.setCursor(18, 2);
      lcd.print(checkstatus4);
      delay(10);
    }

    ///////  ล้อคที่ 5 
    if (checkstatus5 == 1) {  // เช้คค่า ถ้าตัวแปล checkstatus5 = 1 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(6, 3);
      lcd.print("LOG 5:");
      lcd.setCursor(13, 3);
      lcd.print(checkstatus5);
      delay(10);
    }
    else if (checkstatus5 == 0) {  // เช้คค่า ถ้าตัวแปล checkstatus5 = 0 ให้ทำงานที่อยู่ภายใต้เงื่อนไข
      lcd.setCursor(6, 3);
      lcd.print("LOG 5:");
      lcd.setCursor(13, 3);
      lcd.print(checkstatus5);
      delay(10);
    }
  }
}
