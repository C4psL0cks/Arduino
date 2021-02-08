#define PWM_STEERING  6
#define IN2_STEERING  7
#define IN1_STEERING  8

#define PWM_BREAK  9
#define IN2_BREAK  10
#define IN1_BREAK  11

void setup() {

  Serial.begin(115200);
  pinMode(PWM_STEERING, OUTPUT);
  pinMode(IN2_STEERING, OUTPUT);
  pinMode(IN1_STEERING, OUTPUT);

  pinMode(PWM_BREAK, OUTPUT);
  pinMode(IN2_BREAK, OUTPUT);
  pinMode(IN1_BREAK, OUTPUT);

}

void loop() {

}
