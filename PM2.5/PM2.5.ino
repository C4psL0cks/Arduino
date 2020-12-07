#include "PMS.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

PMS pms(Serial);
PMS::DATA data;

String pm25_;
String pm10_;
String response;



void setup() {

  Serial.begin(9600);
  pms.passiveMode(); // Switch to passive mode

  lcd.init();
  lcd.backlight();
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, World!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(0, 1);
  lcd.print("Hello, World!");
  delay(1000);
  lcd.clear();

}
void loop() {
  pms.wakeUp();
  delay(30000);
  pms.requestRead();
  Serial.println("Wait max. 1 second for read…");
  if (pms.readUntil(data)) {
    Serial.print("PM 1.0 (ug / m3): ");
    Serial.println(data.PM_AE_UG_1_0);
    Serial.print("PM 2.5 (ug / m3): ");
    Serial.println(data.PM_AE_UG_2_5);
    Serial.print("PM 10.0 (ug / m3):");
    Serial.println(data.PM_AE_UG_10_0);

    pm25_ = String(data.PM_AE_UG_2_5 * 0.66); //จากการเทียบค่า Sensirion
    pm10_ = String(data.PM_AE_UG_10_0 * 0.66); //จากการเทียบค่า Sensirion

    lcd.setCursor(0, 0);
    lcd.print(pm25_);
    lcd.setCursor(2, 1);
    lcd.print(pm10_);
  }
  else {
    Serial.println("No data.");
  }
  delay(3000);
}
