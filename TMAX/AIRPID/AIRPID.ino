#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <PID_v1.h>
DHT dht;
#define PIN_OUTPUT 3
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
double Setpoint, Feedback, Output;
double Kp = 15, Ki = 0, Kd = 0;
int pushButton = 11;
int pushButton2 = 10;
int k = 0;
int k2 = 0;
int set = 25;
int maxtem = 30;
int mintem = 18;
PID myPID(&Feedback, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);
void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  myPID.SetOutputLimits(0, 255);
  dht.setup(5);
}
void loop() {
  int temp = dht.getTemperature();
  int buttonState = digitalRead(pushButton);
  int buttonState2 = digitalRead(pushButton2);
  if (buttonState == 0 && k == 0) {
    set = set + 1;
    k = 1 ;
  }
  else if (buttonState == 1 && k == 1) {
    k = 0 ;
  }
  if (buttonState2 == 0 && k2 == 0) {
    set = set - 1;
    k2 = 1 ;
  }
  else if (buttonState2 == 1 && k2 == 1) {
    k2 = 0 ;
  }
  set = constrain(set, mintem, maxtem);
  Setpoint = set;
  Feedback =  temp;
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
  int OUT = (Output / 255) * 100;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(14, 10);
  display.println("Set");
  display.setCursor(14, 40);
  display.println("Car");
  display.setCursor(50, 10);
  display.println("Compressor");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println(set);
  display.setCursor(10, 50);
  display.println(temp);
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(45, 25);
  display.print(OUT);
  display.println("%");
  display.display();

}
