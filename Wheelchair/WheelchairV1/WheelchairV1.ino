int upper = 712 ;
int lowwer = 312 ;
//motorL
int mL_EL = 8;  //EL - Start_Stop
int mL_M  = 2;  //Signal - Hall sensor
int mL_ZF = 4;  //ZF - Direction
int mL_VR = 5;  //VR - speed
int mSpeedL = 0;

//motorR
int mR_EL = 9;  //EL - Start_Stop
int mR_M  = 3;  //Signal - Hall sensor
int mR_ZF = 7;  //ZF - Direction
int mR_VR = 6;  //VR - speed
int mSpeedR = 0;
//สวิตจอย
#include <ButtonDebounce.h>
int sw = 2 ;
int g = 85;
ButtonDebounce button(2, 250);

void setup()
{
  pinMode(mL_EL, OUTPUT);
  pinMode(mL_M, INPUT);
  pinMode(mL_ZF, OUTPUT);
  pinMode(mL_VR, OUTPUT);

  pinMode(mR_EL, OUTPUT);
  pinMode(mR_M, INPUT);
  pinMode(mR_ZF, OUTPUT);
  pinMode(mR_VR, OUTPUT);

  pinMode(sw, INPUT_PULLUP);
  //  pinMode(PIN_A, INPUT);
  //  digitalWrite(PIN_A, HIGH);
  button.setCallback(buttonChanged);

  Serial.begin(9600);

}

void buttonChanged(int state) {
  Serial.print("Changed: " + String(state));
  if (state == 0) {
    if (g == 255) {
      g = 85;
    }
    else if (g == 85) {
      g = 170;
    }
    else if (g == 170)
    {
      g = 255;
    }

  }

}

void loop() {
  button.update();
  int xAxis = analogRead(A0); // Read Joysticks X-axis
  int yAxis = analogRead(A1); // Read Joysticks Y-axis
  //เดินหน้า
  if ((yAxis >= upper) && (lowwer < xAxis) && (xAxis < upper)) {
    digitalWrite(mL_EL, LOW);
    digitalWrite(mL_ZF, LOW);
    digitalWrite(mR_EL, LOW);
    digitalWrite(mR_ZF, HIGH);
    mSpeedL = map(yAxis, upper, 1023, 0, g);
    mSpeedR = map(yAxis, upper, 1023, 0, g);
  }
  //ถอยหลัง
  else if ((yAxis <= lowwer) && (lowwer < xAxis) && (xAxis < upper)) {
    digitalWrite(mL_EL, LOW);
    digitalWrite(mL_ZF, HIGH);
    digitalWrite(mR_EL, LOW);
    digitalWrite(mR_ZF, LOW);
    mSpeedL = map(yAxis, lowwer, 0, 0, g);
    mSpeedR = map(yAxis, lowwer, 0, 0, g);
  }
  //เลี้ยวซ้าย-สลับล้อ
  else if ((xAxis <= lowwer) && (lowwer < yAxis) && (yAxis < upper)) {
    digitalWrite(mL_EL, LOW);
    digitalWrite(mL_ZF, HIGH);
    digitalWrite(mR_EL, LOW);
    digitalWrite(mR_ZF, HIGH);
    mSpeedL = map(xAxis, lowwer, 0, 0, g);
    mSpeedR = map(xAxis, lowwer, 0, 0, g);
  }
  //เลี้ยวขวา-สลับล้อ
  else if ((xAxis >= upper) && (lowwer < yAxis) && (yAxis < upper)) {
    digitalWrite(mL_EL, LOW);
    digitalWrite(mL_ZF, LOW);
    digitalWrite(mR_EL, LOW);
    digitalWrite(mR_ZF, LOW);
    mSpeedL = map(xAxis, upper, 1023, 0, g);
    mSpeedR = map(xAxis, upper, 1023, 0, g);
  }
  //เดินหน้า-ซ้าย
  else if ((yAxis >= upper) && (xAxis <= lowwer)) {
    digitalWrite(mL_EL, HIGH);
    digitalWrite(mL_ZF, LOW);
    digitalWrite(mR_EL, HIGH);
    digitalWrite(mR_ZF, HIGH);
    mSpeedL = map(xAxis, lowwer, 0, g, 0);
    mSpeedR = map(yAxis, upper, 1023, 0, g);
  }
  //เดินหน้า-ขวา
  else if ((yAxis >= upper) && (xAxis >= upper)) {
    digitalWrite(mL_EL, HIGH);
    digitalWrite(mL_ZF, LOW);
    digitalWrite(mR_EL, HIGH);
    digitalWrite(mR_ZF, HIGH);
    mSpeedL = map(yAxis, upper, 1023, 0, g);
    mSpeedR = map(xAxis, upper, 1023, g, 0);
  }
  //ถอยหลัง-ซ้าย
  else if ((yAxis <= lowwer) && (xAxis <= lowwer)) {
    digitalWrite(mL_EL, HIGH);
    digitalWrite(mL_ZF, HIGH);
    digitalWrite(mR_EL, HIGH);
    digitalWrite(mR_ZF, LOW);
    mSpeedL = map(yAxis, lowwer, 0, 0, g);
    mSpeedR = map(xAxis, lowwer, 0, g, 0);
  }
  //ถอยหลัง-ขวา
  else if ((yAxis <= lowwer) && (xAxis >= upper)) {
    digitalWrite(mL_EL, HIGH);
    digitalWrite(mL_ZF, HIGH);
    digitalWrite(mR_EL, HIGH);
    digitalWrite(mR_ZF, LOW);
    mSpeedL = map(xAxis, upper, 1023, g, 0);
    mSpeedR = map(yAxis, lowwer, 0, 0, g);
  }
  else {
    digitalWrite(mL_EL, HIGH);
    digitalWrite(mR_EL, HIGH);
    mSpeedL = 0;
    mSpeedR = 0;
  }

  analogWrite(mL_VR, mSpeedL); // Send PWM signal to motor A
  analogWrite(mR_VR, mSpeedR); // Send PWM signal to motor B

  //
  Serial.print(mSpeedL);
  Serial.print(" ");
  Serial.print(mSpeedR);
  Serial.print(" ");
  Serial.print(yAxis);
  Serial.print(" ");
  Serial.println(xAxis);

  //  Serial.print ("       XUP : ");
  //  Serial.print (yAxis);
  //  Serial.print ("       XDown : ");
  //  Serial.print (yAxis);
  //
  //  Serial.print ("       Yleft : ");
  //  Serial.print (xAxis);
  //  Serial.print ("       Yright : ");
  //  Serial.println(xAxis);

  //delay(500);

}
