#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int sensorPin = A0;
int ledPin = 13;
int sensorValue = 0;
int Spk = 12;

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(Spk, OUTPUT);
  lcd.begin();
  lcd.home();
}

void loop() {

  sensorValue = analogRead(sensorPin);
  Serial.print("Moisture of Soil:  ");
  Serial.println(sensorValue);
  
  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.print(sensorValue);
  lcd.println("               ");
  lcd.setCursor(0, 1);
 
  if (analogRead(sensorPin) > 600)  /// แห้ง
  {
    lcd.println("S:Air             ");
    SPON();
    LEDOFF();
  }
  if (analogRead(sensorPin) > 500 && analogRead(sensorPin) < 600)
  {
    lcd.println("S:Dry             ");
    SPOFF();
    LEDON();
  }
  if (analogRead(sensorPin) > 200 && analogRead(sensorPin) < 500)
  {
    lcd.println("S:Humid           ");
    SPOFF();
    LEDON();
  }
  if (analogRead(sensorPin) < 200)   // เปียก
  {
    lcd.println("S:Water           ");
    SPOFF();
    LEDON();
  }
  delay(500);
}
void LEDON()
{
  digitalWrite(ledPin, HIGH);
}
void LEDOFF()
{
  digitalWrite(ledPin, LOW);
}
void SPON()
{
  digitalWrite(Spk, HIGH);
}
void SPOFF()
{
  digitalWrite(Spk, LOW);
}








