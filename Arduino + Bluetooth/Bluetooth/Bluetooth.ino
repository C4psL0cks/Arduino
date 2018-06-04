
#include <SoftwareSerial.h>
#include <L298N_ZEREF.h>
SoftwareSerial mySerial(5, 6); // RX, TX


const int pingPin = 7;
int inPin = 4;
int LEDsensor = 2;
long duration, cm;

int LEDbluetooth = A0;
char check;


void setup()
{
  ///////LED
  pinMode(LEDbluetooth, OUTPUT);
  pinMode(LEDsensor, OUTPUT);
  /////// Bluetooth
  Serial.begin(9600);
  while (!Serial) ;
  mySerial.begin(9600);
  /////// motor
  pinMode(10, OUTPUT);  //กำหนด ขา 10 (ต่ออยู่กับ ENA ให้เป็นแบบ OUTPUT)
  pinMode(9, OUTPUT);  //กำหนด ขา 9 (ต่ออยู่กับ IN1 ให้เป็นแบบ OUTPUT)
  pinMode(8, OUTPUT);  //กำหนด ขา 8 (ต่ออยู่กับ IN2 ให้เป็นแบบ OUTPUT)

  /////
  pinMode(inPin, INPUT);
  pinMode(pingPin, OUTPUT);
}

void loop()
{
  ///////////////////////////////////
  //long duration, cm;

  // pinMode(pingPin, OUTPUT);

  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);


  // pinMode(inPin, INPUT);
  
    duration = pulseIn(inPin, HIGH);

    cm = microsecondsToCentimeters(duration);
    if (cm < 100)//เข้าใกล้เกินไปให้หยุด
    {
      Serial.print("Stop");
      ledoffsensor();
      motorstop();
    }
    else
    {
      //cm >= 100 ใกล้เกินไป วิ่งตลอด
      Serial.print("Run");
      ledonsensor();
      motorgo();

    }
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    delay(30);
  ////////////////////////////////
  if (mySerial.available())
  {
    check = mySerial.read();
    if (check == 'n')
    {
      Serial.println("System on!!");
      ledonBluetooth(); //open
      ////////////////////////////
      
      ///////////////////////////
    }
    else if (check == 'f')
    {
      Serial.println("System off!!");
      ledoffBluetooth(); //close
    }

  }
  ///////////////
  if (Serial.available())
  {
    mySerial.write(Serial.read());
  }
  /////////////////
}
void ledonBluetooth()
{
  digitalWrite(LEDbluetooth, HIGH);
}
void ledoffBluetooth()
{
  digitalWrite(LEDbluetooth, LOW);
}
void ledonsensor()
{
  digitalWrite(LEDsensor, HIGH);
}
void ledoffsensor()
{
  digitalWrite(LEDsensor, LOW);
}
void motorgo()
{
  //digitalWrite(9, HIGH);     //ตามตารางข้างบน ต้องกำหนด IN1 = HIGH
 // digitalWrite(8, LOW);      //และ IN2 = LOW มอเตอร์ A จึงจะทำงานหมุนไปด้านหน้า
  //analogWrite(10, 255);      //และสั่งหมุนที่ความเร็วสูงสุด 255 ผ่านทาง ENA (ขา 10) ที่เป็น PWM

  digitalWrite(13, HIGH);     
  digitalWrite(12, LOW);      
  analogWrite(11, 255);      

}
void motorstop()
{
  digitalWrite(9, HIGH);      //ตามตารางข้างบน ต้องกำหนด IN1 = HIGH
  digitalWrite(8, HIGH);      //และ IN2 = HIGH มอเตอร์ A จึงจะหยุดการทำงานทันที (เบรค)
  analogWrite(10, 0);           //และสั่งความเร็วมอเตอร์ให้เป็น 0 ผ่านทาง ENA (ขา 10) ที่เป็น PWM

  digitalWrite(13, HIGH);    
  digitalWrite(12, HIGH);     
  analogWrite(11, 0);     
}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}


