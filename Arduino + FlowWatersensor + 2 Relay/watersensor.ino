#define RELAY1  8            // ขาสัญญาน Delay
#define RELAY2  9            // ขาสัญญาน Delay
const int buttonPin = 4;     //กำหนดขา switch
const int buttonPin2 = 7;    //กำหนดขา switch
int buttonState = 0;         // กำหนด เก็บค่าสถานะของ state
int buttonState2 = 0;        // กำหนด เก็บค่าสถานะของ state
int data;                    // กำหนด  ตัวแปลสำหรับเก็บค่า ของ switch ช่อง off หรือ 0
int data1;                   // กำหนด ตัวแปลสำหรับเก็บค่า ของ switch ช่อง on1 หรือ 1
int data2;                   // กำหนด ตัวแปลสำหรับเก็บค่า ของ switch ช่อง on2 หรือ 2
volatile int NbTopsFan;      // รอนับรอบการหมุนของ waterflowsensor
int Calc;                    // กำหนด รอนับระดับน้ำ waterflowsensor
int hallsensor = 2;          // กำหนดขาของ waterflowsensor
int checkrob1 = 0;            // กำหนด ค่าสถานะการเช้ครอบ
int checkrob2 = 0;            // กำหนด ค่าสถานะการเช้ครอบ

void setup()
{
  Serial.begin(9600);       // กำหนดค่าการอ่านหรือส่งของพอต์
  pinMode(RELAY1, OUTPUT);  // กำหนดค่าสัญญานขาออก Delay
  pinMode(RELAY2, OUTPUT);  // กำหนดค่าสัญญานขาออก Delay
  pinMode(buttonPin, INPUT); // กำหนดค่าสัญญานขาเข้า switch
  pinMode(buttonPin2, INPUT); // กำหนดค่าสัญญานขาเข้า switch
  pinMode(hallsensor, INPUT); // กำหนดค่าสัญญานขาเข้า watersensor
  attachInterrupt(0, rpm, RISING); // กำหนด interupt เมื่อมีสัญญานขาเข้า = 1 จะกระโดดไปทำงานที่ Function ทันที

}
void loop()
{
  int names = switchs(data, data1, data2);  // Function switchs(data=1, data1=2, data2 =0);
  NbTopsFan = 0;                            //Set NbTops to 0 ready for calculations
  sei();                                    //Enables interrupts
  delay (1000);                             //Wait 1 second
  cli();                                    //Disable interrupts
  Calc = (NbTopsFan * 60 / 7.5);            //(Pulse frequency x 60) / 5.5Q, = flow rate in L / hour
  Serial.print (Calc, DEC);                 //Prints the number calculated above
  Serial.print (" L/hour\r\n");             //Print L/hour\r\n

  switch (names)                            // check switch ว่าเป็น  switch ไหน
  {
    case 1:                                 // ถ้าเป็น switch 1 จะทำใน case 1
      if (Calc <= 0 && checkrob1 == 0)      // ถ้าระดับน้ำมันเป็น 0 แลัวรอบสวิตเป็น 0 จะทำเงื่อนไขข้างล่าง
      {
        interupon();                        //สั่ง ฟั่งชั่น delay interupon เปิดน้ำ
        checkrob1 = 1;                      // set ค่ารอบ ให้ = 1
        Serial.println("==========");
        Serial.println("ON1");
        Serial.print("SW1 : ");
        Serial.println(checkrob1);
        Serial.println("==========");

      }
      else if (Calc >= 120 || checkrob1 >= 2)  // ถ้าระดับน้ำมันเป็น 120 แลัวรอบสวิตเป็น 2 โดยการนำค่า 121 มาเช้ค จะทำเงื่อนไขข้างล่าง
      {
        interupoff();                       //สั่ง ฟั่งชั่น delay interupoff ปิดน้ำ
        checkrob1 = checkrob1 + Calc ;      // นำค่ารอบ ที่ = 1 ไป + กับ ค่า calc คือระดับน้ำ จะได้ 120 + 1 = 121
        Serial.println("==========");       // print "=========="
        Serial.println("OFF1");             // print "OFF1"
        Serial.print("SW1 : ");             // print "SW1 : "
        Serial.println(checkrob1);          // print ค่า รอบ
        Serial.println("==========");       // print "=========="

      }
      break;
    case 2:                                 // ถ้าเป็น switch 2 จะทำใน case 2
      if (Calc <= 0 && checkrob2 == 0)      // ถ้าระดับน้ำมันเป็น 0 แลัวรอบสวิตเป็น 0 จะทำเงื่อนไขข้างล่าง
      {
        interupon();                        //สั่ง ฟั่งชั่น delay interupon เปิดน้ำ
        checkrob2 = 1;                      // set ค่ารอบ ให้ = 1
        Serial.println("==========");       // print "=========="
        Serial.println("ON2");              // print "ON2"
        Serial.print("SW2 : ");             // print "SW2 : "
        Serial.println(checkrob2);          // print ค่า รอบ
        Serial.println("==========");       // print "=========="
      }
      else if (Calc >= 222 || checkrob2 >= 2) // ถ้าระดับน้ำมันเป็น 120 แลัวรอบสวิตเป็น 2 โดยการนำค่า 223 มาเช้ค จะทำเงื่อนไขข้างล่าง
      {
        interupoff();                       //สั่ง ฟั่งชั่น delay interupoff ปิดน้ำ
        checkrob2 = checkrob2 + Calc;       // นำค่ารอบ ที่ = 1 ไป + กับ ค่า calc คือระดับน้ำ จะได้ 222 + 1 = 223
        Serial.println("==========");       // print "=========="
        Serial.println("OFF2");             // print "OFF2"
        Serial.print("SW2 : ");             // print "SW2 : "
        Serial.println(checkrob2);          // print ค่า รอบ
        Serial.println("==========");       // print "=========="
      }
      break;
    case 0:                                 // ถ้าเป็น switch 0 จะทำใน case 0
      interupoff();                         //สั่ง ฟั่งชั่น delay interupoff ปิดน้ำ
      checkrob1 = 0;                        // set ค่ารอบ ให้ = 0
      checkrob2 = 0;                        // set ค่ารอบ ให้ = 0
      Serial.println("==========");         // print "=========="
      Serial.println("OFF System");         // print "OFF System"
      Serial.print("SW1 : ");               // print "SW1"
      Serial.println(checkrob1);            // print ค่า รอบ
      Serial.print("SW2 : ");               // print "SW2"
      Serial.println(checkrob2);            // print ค่า รอบ
      Serial.println("==========");         // print "=========="
      break;
  }
}
int switchs(int data, int data1, int data2) // Function switch เก็ยค่าสถานะของ switch
{
  buttonState = digitalRead(buttonPin);     //Reading switch1
  buttonState2 = digitalRead(buttonPin2);   //Reading  switch2

  if (buttonState == HIGH)                  //Reading switch1 if == HIGH set data = 1
  {
    data = 1;                               //set data = 1
    return data;                            //return data
  }
  else if (buttonState2 == HIGH)            //Reading switch2 if == HIGH  set data = 1
  {
    data1 = 2;                              //set data = 1
    return data1;                           //return data
  }
  else if (buttonState == LOW || buttonState2 == LOW) //Reading switch1 and Reading switch2 if == LOW set data = 0
  {
    data2 = 0;                              //set data = 0          
    return data2;                           //return data
  }
} 
void interupon()                            // Function interupon
{
  digitalWrite(RELAY1, LOW);                // Turns Relay Off
  digitalWrite(RELAY2, LOW);                // Turns Relay Off
}
void interupoff()                           // Function interupon
{
  digitalWrite(RELAY1, HIGH);               // Turns Relay Off
  digitalWrite(RELAY2, HIGH);               // Turns Relay Off
}
void rpm ()                                 // Function rmp
{
  NbTopsFan++;                              // บวกค่ารอบ ของ waterflowsensor
}
