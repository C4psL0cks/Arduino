#define PWM_SPEED  11
#define VRCB   A3
int SPEED;
void setup() {
  Serial.begin(115200);
  pinMode(PWM_SPEED, OUTPUT);
}

void loop() {

  //  Serial.println(analogRead(VRCB));
  SPEED = constrain(analogRead(VRCB), 0, 1024);
  //  Serial.println(SPEED);
  SPEED = map(SPEED, 0, 1024, 0, 255);
  Serial.println(SPEED);
  analogWrite(PWM_SPEED, SPEED);

}
