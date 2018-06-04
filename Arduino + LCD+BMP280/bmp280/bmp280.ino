#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
Adafruit_BMP280 bme;
LiquidCrystal_I2C lcd(0x27, 16, 2); //

void setup()
{
  Serial.begin(9600);
  if (!bme.begin())
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  lcd.begin();
  lcd.home();
}
void loop()
{

  Serial.print("--------------------------------\n\n");
  Serial.print("T = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C    ");
  Serial.print("P = ");
  Serial.print(bme.readPressure() / 100); // 100 Pa = 1 millibar
  Serial.println(" mb    ");
  Serial.print("Approx altitude = ");
  Serial.print(bme.readAltitude(1013.25));
  Serial.println(" m");
  Serial.print("--------------------------------\n\n");

  lcd.print("T = ");
  lcd.print(bme.readTemperature());
  lcd.println(" *C    ");
  lcd.setCursor(0, 1);
  lcd.print("P = ");
  lcd.print(bme.readPressure() / 100); // 100 Pa = 1 millibar
  lcd.println(" mb    ");
  lcd.setCursor(0, 2);
  delay(2000);
}

