const int pingPin = 13;
int inPin = 12;
#define RELAY1  8
#define RELAY2  9
int LEDHIGH = 2;
int LEDMID = 3;
int LEDLOW = 4;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(LEDHIGH, OUTPUT);
  pinMode(LEDMID, OUTPUT);
  pinMode(LEDLOW, OUTPUT);
}

void loop()
{
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);

  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.println("cm");
  if (cm <= 20)  // stop
  {
    interupoff();
    LEDON1();
    LEDOFF2();
    LEDOFF3();
    Serial.println("HIGH");
  }
  else if (cm == 31 && cm <= 40) 
  {
    interupon();
    LEDON2();
    LEDOFF1();
    LEDOFF3();
    Serial.println("MID");
  }
  else if (cm <= 40 && cm > 30)
  {
    interupon();
    LEDON3();
    LEDOFF1();
    LEDOFF2();
    Serial.println("LOW");
  }
  delay(600);
}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
void LEDON1()
{
  digitalWrite(LEDHIGH, HIGH);
}
void LEDOFF1()
{
  digitalWrite(LEDHIGH, LOW);
}
void LEDON2()
{
  digitalWrite(LEDMID, HIGH);
}
void LEDOFF2()
{
  digitalWrite(LEDMID, LOW);
}
void LEDON3()
{
  digitalWrite(LEDLOW, HIGH);
}
void LEDOFF3()
{
  digitalWrite(LEDLOW, LOW);
}
void interupoff()
{
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
}
void interupon()
{
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}
