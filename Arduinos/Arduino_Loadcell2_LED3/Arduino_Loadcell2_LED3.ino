#include "HX711.h"

float calibration_factor1 = 76436.00;
#define zero_factor1 8428268

float calibration_factor2 = 100518.00;
#define zero_factor2 8297276

#define DOUT1  A3
#define CLK1   A2

#define DOUT2  A5
#define CLK2   A4

int ledPin1 = 2; // Red
int ledPin2 = 3; // yellow
int ledPin3 = 4; // green

float sum = 0 ; //operation
int va = 2 ;  //operation


float offset1 = 0;
float get_units_kg1(); // load 1

float offset2 = 0;
float get_units_kg2(); // load 2

HX711 scale1(DOUT1, CLK1); // load 1
HX711 scale2(DOUT2, CLK2); // load 2

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  //Serial.println("Load Cell1");
  scale1.set_scale(calibration_factor1);
  scale1.set_offset(zero_factor1);
  //Serial.println("Load Cell2");
  scale2.set_scale(calibration_factor2);
  scale2.set_offset(zero_factor2);
  
}
void loop()
{
  //Serial.print("Readin1: ");
  float load1 = get_units_kg1() + offset1; // x + 0
  //Serial.print(load1);
  //Serial.println(" kg");
  //Serial.print("Reading2: ");
  float load2 = get_units_kg2() + offset2;
  //Serial.print(load2);
  //Serial.println(" kg");

  sum = load1 + load2;
  sum = sum / va;
  Serial.println(sum);

  if (sum <= 3.0)
  {
    ledon1();
    ledoff2();
    ledoff3();
  }
  else if (sum > 3.0 && sum < 5.0)
  {
    ledon2();
    ledoff1();
    ledoff3();
  }
  else if (sum >= 5.0)
  {
    ledon3();
    ledoff1();
    ledoff2();
  }
}

void ledon1()
{
  digitalWrite(ledPin1, HIGH);
  delay(100);

}
void ledoff1()
{
  digitalWrite(ledPin1, LOW);
  delay(100);
}
void ledon2()
{
  digitalWrite(ledPin2, HIGH);
  delay(100);

}
void ledoff2()
{
  digitalWrite(ledPin2, LOW);
  delay(100);
}
void ledon3()
{
  digitalWrite(ledPin3, HIGH);
  delay(100);

}
void ledoff3()
{
  digitalWrite(ledPin3, LOW);
  delay(100);
}
float get_units_kg1()
{
  return (scale1.get_units() * 0.453592);
}
float get_units_kg2()
{
  return (scale2.get_units() * 0.453592);
}
