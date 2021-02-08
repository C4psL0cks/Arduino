#include <PID_v1.h>
#define EN_BROW           23
#define EN_RED            25
#define EN_ORANGE         27
#define EN_YELLOW         29
#define EN_BLUE           31
#define EN_PURPLE         33
#define EN_GREY           35
#define EN_WHITE_BROW     37
#define EN_WHITE_RED      39
#define EN_WHITE_ORANGE   41
#define EN_WHITE_YELLOW   43
#define EN_WHITE_BLUE     45
#define EN_WHITE_PURPLE   47

double Setpoint, Feedback, Output, Kp = 5, Ki = 0, Kd = 0;
int EN0 = 0, EN1 = 0, EN2 = 0, EN3 = 0, EN4 = 0, EN5 = 0, EN6 = 0, EN7 = 0, EN8 = 0, EN9 = 0, EN10 = 0, EN11 = 0, EN12 = 0;
PID myPID(&Feedback, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int SUM;
void setup() {

  Serial.begin(115200);

  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  myPID.SetOutputLimits(-150, 150);

  pinMode(EN_BROW, INPUT_PULLUP);
  pinMode(EN_RED, INPUT_PULLUP);
  pinMode(EN_ORANGE, INPUT_PULLUP);
  pinMode(EN_YELLOW, INPUT_PULLUP);
  pinMode(EN_BLUE, INPUT_PULLUP);
  pinMode(EN_PURPLE, INPUT_PULLUP);
  pinMode(EN_GREY, INPUT_PULLUP);
  pinMode(EN_WHITE_BROW, INPUT_PULLUP);
  pinMode(EN_WHITE_RED, INPUT_PULLUP);
  pinMode(EN_WHITE_ORANGE, INPUT_PULLUP);
  pinMode(EN_WHITE_YELLOW, INPUT_PULLUP);
  pinMode(EN_WHITE_BLUE, INPUT_PULLUP);
  pinMode(EN_WHITE_PURPLE, INPUT_PULLUP);

}

void loop() {

  EN0 = digitalRead(EN_BROW)           * 1;
  EN1 = digitalRead(EN_RED)            * 2;
  EN2 = digitalRead(EN_ORANGE)         * 4;
  EN3 = digitalRead(EN_YELLOW)         * 8;
  EN4 = digitalRead(EN_BLUE)           * 10;
  EN5 = digitalRead(EN_PURPLE)         * 20;
  EN6 = digitalRead(EN_GREY)           * 40;
  EN7 = digitalRead(EN_WHITE_BROW)     * 80;
  EN8 = digitalRead(EN_WHITE_RED)      * 100;
  EN9 = digitalRead(EN_WHITE_ORANGE)   * 200;
  EN10 = digitalRead(EN_WHITE_YELLOW)  * 400;
  EN11 = digitalRead(EN_WHITE_BLUE)    * 800;
  EN12 = digitalRead(EN_WHITE_PURPLE)  * 1000;
  //
  //  Serial.println(EN0);
  //  Serial.println(EN1);
  //  Serial.println(EN2);
  //  Serial.println(EN3);
  //  Serial.println(EN4);
  //  Serial.println(EN5);
  //  Serial.println(EN6);
  //  Serial.println(EN7);
  //  Serial.println(EN8);
  //  Serial.println(EN9);
  //  Serial.println(EN10);
  //  Serial.println(EN11);
  //  Serial.println(EN12);
  //  delay(100);

  SUM = EN0 + EN1 + EN2 + EN3 + EN4 + EN5 + EN6 + EN7 + EN8 + EN9 + EN10 + EN11 + EN12;
  //  Serial.println(SUM);
  //  int ss = SUM % 1000;
  Feedback = SUM % 1000;
  //  Serial.println(ss);
  double error = Setpoint - Feedback;
  //  myPID.Compute();
  //  Serial.println(Output);
  Serial.println(Feedback);
  //  Serial.println(error);
  delay(100);

}
