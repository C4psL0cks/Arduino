// SETTING
#define LOWER_LEFT_RIGHT  1000
#define UPPER_LEFT_RIGHT  1900

//(1003,1490)
#define LOWER_BREAK       1010
#define UPPER_BREAK       1490

//(1019,1974)
#define LOWER_SPEED       1020
#define UPPER_SPEED       1970

//(995,1989)
#define LOWER_ON_OFF      1000
#define UPPER_ON_OFF      1980

//(995,1989)
#define LOWER_FRONT_BACK  1000
#define UPPER_FRONT_BACK  1980

// REMOTE
int CH1_LEFT_RIGHT, CH2_BREAK, CH3_SPEED, CH5_ON_OFF, CH6_FRONT_BACK;

void setup() {

  Serial.begin(115200);

  // REMOTE
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
}

void loop() {

  CH1_LEFT_RIGHT = pulseIn(A8, HIGH, 250000); // left 1039 / center 1515 / right 1989
  CH2_BREAK = pulseIn(A9, HIGH, 250000); // center 1496 / down 1004
  CH3_SPEED = pulseIn(A10, HIGH, 250000); // down 1018 / up 1971
  CH5_ON_OFF = pulseIn(A11, HIGH, 250000); // on 995 / off 1989
  CH6_FRONT_BACK = pulseIn(A12, HIGH, 250000); // f 995 b 1989

  Serial.println("-------------------------------");
  Serial.println("CH1_LEFT_RIGHT: " + String(CH1_LEFT_RIGHT));
  Serial.println("CH2_BREAK: " + String(CH2_BREAK));
  Serial.println("CH3_SPEED: " + String(CH3_SPEED));
  Serial.println("CH5_ON_OFF: " + String(CH5_ON_OFF));
  Serial.println("CH6_FRONT_BACK: " + String(CH6_FRONT_BACK));
  Serial.println("-------------------------------");

  //  L / R
  CH1_LEFT_RIGHT = constrain(CH1_LEFT_RIGHT, LOWER_LEFT_RIGHT , UPPER_LEFT_RIGHT);
  Serial.println("CH1_LEFT_RIGHT: " + String(CH1_LEFT_RIGHT));
  CH1_LEFT_RIGHT = map(CH1_LEFT_RIGHT, LOWER_LEFT_RIGHT , UPPER_LEFT_RIGHT, 80, 980);

  // BREAK
  CH2_BREAK = constrain(CH2_BREAK, LOWER_BREAK , UPPER_BREAK);
  Serial.println("CH2_BREAK: " + String(CH2_BREAK));
  CH2_BREAK = map(CH2_BREAK, LOWER_BREAK , UPPER_BREAK, 1, 0); // no break 0 break 1

  // SPEED
  CH3_SPEED = constrain(CH3_SPEED, LOWER_SPEED , UPPER_SPEED);
  Serial.println("CH3_SPEED: " + String(CH3_SPEED));
  CH3_SPEED = map(CH3_SPEED, LOWER_SPEED , UPPER_SPEED, 0, 255);
  analogWrite(PWM_SPEED, CH3_SPEED);

  // ON / OFF
  CH5_ON_OFF = constrain(CH5_ON_OFF, LOWER_ON_OFF , UPPER_ON_OFF);
  Serial.println("CH5_ON_OFF: " + String(CH5_ON_OFF));
  CH5_ON_OFF = map(CH5_ON_OFF, LOWER_ON_OFF , UPPER_ON_OFF, 0, 1);
  digitalWrite(IN1_ON_OFF, CH5_ON_OFF);
  digitalWrite(IN3_SELECTER, HIGH);

  // FRONT / BACK
  CH6_FRONT_BACK = constrain(CH6_FRONT_BACK, LOWER_FRONT_BACK , UPPER_FRONT_BACK);
  Serial.println("CH6_FRONT_BACK: " + String(CH6_FRONT_BACK));
  CH6_FRONT_BACK = map(CH6_FRONT_BACK, LOWER_FRONT_BACK , UPPER_FRONT_BACK, 0, 1);
  digitalWrite(IN2_FONT_BACK, CH6_FRONT_BACK);

  Serial.println(CH1_LEFT_RIGHT);
  Serial.println(CH2_BREAK);
  Serial.println(CH3_SPEED);
  Serial.println(CH5_ON_OFF);
  Serial.println(CH6_FRONT_BACK);
  delay(300);

}
