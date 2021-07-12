//#define PWM_SPEED  12
#define SN_BREAK   13

void setup() {

  Serial.begin(115200);
  //  pinMode(PWM_SPEED, OUTPUT);
  pinMode(SN_BREAK, INPUT);
  //  pinMode(2, OUTPUT);
  //  digitalWrite(2, LOW);


}

void loop() {

  // break
  Serial.println(digitalRead(SN_BREAK));
  delay(500);

  // pwm
  //  analogWrite(PWM_SPEED, 200);

}
