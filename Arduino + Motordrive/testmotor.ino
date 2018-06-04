void setup()
{

  pinMode(8, OUTPUT);  //IN3
  pinMode(9, OUTPUT);  //IN4
  pinMode(10, OUTPUT);  //ENB
  pinMode(11, OUTPUT);  //ENA
  pinMode(12, OUTPUT);  //IN1
  pinMode(13, OUTPUT);  //IN2


}

void loop()
{
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  analogWrite(10, 255);
  analogWrite(11, 255);

}

