#include <TridentTD_EasyFreeRTOS_Ardu.h>  // เรียกใช้ไลบารี task

TridentOS  task1, task2, task3, task4, task5, LCD_task;  // ประกาศตัวแปล สำหรับฟังชั้น task
void sonic1(void*), sonic2(void*), sonic3(void*), sonic4(void*), sonic5(void*), LCD_func(void*); // เรีบยกใช้ฟังชั่น task
int checkstatus1 = 0, checkstatus2 = 0, checkstatus3 = 0, checkstatus4 = 0, checkstatus5 = 0;   // ประกาศตัวแปลสำหรับการเช็คค่าสถานะ

void setup() {
  Serial.begin(115200);    // กำหนดพอตการเชื่อมต่อและส่งข้อมูล ที่ 115200
  Serial.println(); 
  task1.start( sonic1, NULL, 56);   // เริ่ม task 1 ให้ไปเรียก sonic1 ทำงาน
  task2.start( sonic2, NULL, 56);   // เริ่ม task 2 ให้ไปเรียก sonic2 ทำงาน
  task3.start( sonic3, NULL, 56);   // เริ่ม task 3 ให้ไปเรียก sonic3 ทำงาน
  task4.start( sonic4, NULL, 56);   // เริ่ม task 4 ให้ไปเรียก sonic4 ทำงาน
  task5.start( sonic5, NULL, 56);   // เริ่ม task 5 ให้ไปเรียก sonic5 ทำงาน
  LCD_task.start(LCD_func, NULL, 128); //เริ่ม task 6 จัดการ หน้าจอ LCD
}
void loop() {}
