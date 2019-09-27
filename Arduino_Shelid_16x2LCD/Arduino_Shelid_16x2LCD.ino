#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int count = 0;
int read_LCD_buttons() {
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;

}
void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello <3");
  Serial.begin(9600);
}
void loop() {

  lcd.setCursor(9, 1);           // move cursor to second line "1" and 9 spaces over
  lcd.print(count);
  lcd.setCursor(0, 1);

  lcd_key = read_LCD_buttons();

  switch (lcd_key) {

    case btnRIGHT:
      lcd.print("RIGHT ");
      break;

    case btnLEFT:
      lcd.print("LEFT   ");
      break;

    case btnUP:
      lcd.print("UP    ");
      break;

    case btnDOWN:
      lcd.print("DOWN  ");
      break;

    case btnSELECT:
      lcd.print("SELECT");
      break;

    case btnNONE:
      lcd.print("NONE  ");
      break;
  }
}
