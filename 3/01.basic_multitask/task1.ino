
void sonic1(void*) {   //ฟังชั่น sensor ตัวที่ 1 

  const int trigPin = 2;  //กำหนด ขา sensor ขา 2
  const int echoPin = 3;  //กำหนด ขา sensor ขา 3
  int led = A0;   // กำหนด ขา led ขา A0

  VOID SETUP() {    // ฟังชั่นย่อย setup มีหน้าที่กำหนดค่าเริ่มต้นต่างๆก่อนเริ่มการทำงาน
    Serial.begin(9600); // กำหนดพอตการเชื่อมต่อและส่งข้อมูล ที่ 9600
  }

  VOID LOOP() {   // ฟังชั่น loop ย่อย มีหน้าที่รันคำสั่งแบบเป็น loop ไปเรื่อยๆ
    long duration1, cm1;  // กำหนดตัวแปล หาค่าเฉลี่ย กำ ตัวแปล cm
    pinMode(led, OUTPUT);  // กำหนดการเชื่อมต่อของขา led แบบ output
    pinMode(trigPin, OUTPUT); // กำหนดการเขื่อมต่อของขา sensor แบบ output
    digitalWrite(trigPin, LOW); // ส่งสัญญาน 0 ออกไปที่ขา sensor
    digitalWrite(trigPin, HIGH); // ส่งสัญญาน 1 ออกไปที่ขา sensor
    delayMicroseconds(5);  // หน่วงเวลา 5 ไมโคเซค
    digitalWrite(trigPin, LOW);  // ส่งสัญญาน 0 ออกไปที่ขา sensor
    pinMode(echoPin, INPUT);  // กำหนดการเขื่อมต่อของขา sensor แบบ imput
    duration1 = pulseIn(echoPin, HIGH);  // นำตัวแปลหาค่าเฉลี่ยมา = ค่าของสัญญานขาเข้าที่ตอบกลับมา 
    cm1 = duration1 / 29 / 2;  // นำค่าเฉลี่ยที่ได้มาเข้าสูตรของ อัลตร้าโซนิก โดยนำค่าที่ได้จากการเฉลี่ยของ sensor มา หาร 29 และหาร 2 และจะได้ cm ออกมา
    
    if (cm1 < 8) {  // จากนั้นเช้คว่า cm นั้นมีระยะห่าง น้อยกว่า 8 cm หรอไม่ ถ้าน้อยกว่าให้ทำงานที่อยู่ภายใต้เงื่อนไข
      
      checkstatus1 = 1;           // set ค่าตัวแปล ให้ เท่ากับ 1 หมายความว่า ช่อง 1 ไม่ว่าง
      digitalWrite(led, LOW);    // ส่งสัญญาน ไปที่ขา led แบบ low ทำให้ไฟดับ
    } else {
      checkstatus1 = 0;           // set ค่าตัวแปล ให้ เท่ากับ 0 หมายความว่า ช่อง 1 ว่าง
      digitalWrite(led, HIGH);   // ส่งสัญญาน ไปที่ขา led แบบ high ทำให้ไฟติด
    }
    delay(100);     // หน่วงเวลา 0.1 วินาที
  }
}
//////////////************ task 2,3,4,5 คอมเมนต์เหมือนกันหมดแค่ เปลี่ยนเซ็นเซอร์ไปแต่ละตัว ถ้า งง อธิบายง่ายๆคือ task 1 คือค่าของเซ็นเซอร์ตัวที่ 1 task 2 3 4 ก็ทำลำดับ
