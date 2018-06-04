#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);// PCF8574A =  0x3F
void setup() {
  // put your setup code here, to run once:
lcd.begin();
lcd.home();
lcd.print("Hello !!!");
lcd.setCursor(0,1);
lcd.print("Arduino Step");
lcd.setCursor(0,2);
lcd.print("www.ArduinoStep.com");
lcd.setCursor(0,3);
lcd.print("FB : ggggg");

}

void loop() {
  // put your main code here, to run repeatedly:

}
