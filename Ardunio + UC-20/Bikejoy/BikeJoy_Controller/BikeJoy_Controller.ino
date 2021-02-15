int Alarm = 7; //OUTPUT

int VR = 5; //OUTPUT - speed
int ZF = 9; //OUTPUT - Direction
int EL = 8; //OUTPUT - Start_Stop
int Signal = 6; //INPUT - Hall sensor

int HDX2 = 4; //INPUT
int Open = 3; //INPUT
int Accelerator = 0; //INPUT
unsigned long duration; //variable
float Speed = 0.0; //variable

void setup()
{
  pinMode(Alarm, OUTPUT);
  
  pinMode(VR, OUTPUT);
  pinMode(ZF, OUTPUT);
  pinMode(EL, OUTPUT);
  pinMode(Signal, INPUT);
  
  pinMode(HDX2, INPUT);
  pinMode(Open, INPUT);
  pinMode(Accelerator, INPUT);
  
  Serial.begin(9600);

}

void loop()
{
  if (digitalRead(Open) == HIGH) //ปลดล็อก
  {
    digitalWrite(EL, HIGH); //เปิดการทำงานคอนโทล
    digitalWrite(ZF, LOW);
    digitalWrite(Alarm, LOW);
    duration = pulseIn(Signal, HIGH);
    Accelerator = analogRead(A0);
    Accelerator = constrain(Accelerator, 190 , 850);
    Accelerator = map(Accelerator, 190, 850, 0, 255);
   
    if (duration <= 0) {
      Speed = 0;
      analogWrite(VR, 0);
    }

    else if (duration > 0 && duration < 8000) {
      Speed = 1000000 / duration;
      Speed = Speed * 0.0132; //km/hr ค่าจริง 0.0151
      analogWrite(VR, Accelerator);
    }
    else {
      analogWrite(VR, 0);
    }

  }

  else if (digitalRead(Open) == LOW) //ล็อก
  {
    digitalWrite(EL, LOW);
    analogWrite(VR, 0);
    if (digitalRead(HDX2) == HIGH)
    {
      digitalWrite(Alarm, HIGH);
      delay(6000);
    }
    else if (digitalRead(HDX2) == LOW)
    {
      digitalWrite(Alarm, LOW);
    }
  }

}
