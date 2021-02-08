#define PWM_SPEED  12
#define VRCB   A0

void setup() {
  Serial.begin(115200);
  pinMode(PWM_SPEED, OUTPUT);
}

void loop() {

  Serial.println(analogRead(VRCB));
  Serial.println(map(analogRead(VRCB), 0, 1024, 0, 255));
  analogWrite(PWM_SPEED, 213);

}
