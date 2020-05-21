#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial bluetooth(2, 3);

const int dataPin = 8;  // blue wire to 74HC595 pin 14
const int latchPin = 9; // green to 74HC595 pin 12
const int clockPin = 12; // yellow to 74HC595 pin 11
const char common = 'a';    // common anode
bool decPt = true;  // decimal point display flag

int incomingcase;
int x = 0;

int sw = 13;
int counter;
int Last_sate;


void setup() {
  Serial.begin(9600);
  // initialize I/O pins
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  while (!Serial);
  bluetooth.begin(9600);
  Serial.println("\nCheck BLE Version 1.0.0\n");
}

void loop() {

  EEPROM.get(0, x);
  EEPROM.end();
  //  Serial.println(x);

  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }
  if (bluetooth.available()) {
    //    Serial.write(bluetooth.read());
    incomingcase = bluetooth.read();
    //    Serial.println(incomingcase);
    switch (incomingcase) {
      case 0:
        Serial.println("CMD_0");
        x = 0;
        break;
      case 11:
        Serial.println("CMD_11");
        x = 1;
        break;
      case 21:
        Serial.println("CMD_21");
        x = 2;
        break;
      case 31:
        Serial.println("CMD_31");
        x = 3;
        break;
      case 41:
        Serial.println("CMD_41");
        break;
      case 51:
        Serial.println("CMD_51");
        break;
      case 60:
        Serial.println("CMD_60");
        break;
      case 61:
        Serial.println("CMD_61");
        break;
      case 70:
        Serial.println("CMD_70");
        break;
      case 71:
        Serial.println("CMD_71");
        break;
    }
  }

  EEPROM.put(0, x);
  EEPROM.end();
  //  Serial.println("Written");

  int reads = digitalRead(sw);
  if(reads == 0 && Last_sate == 1){
    counter += 1;
      if (counter == 16) {
      counter = 0;
    }
  }
  Last_sate = reads;
//  Serial.println(counter);

  decPt = !decPt; // display decimal point every other pass through loop
  byte bits = myfnNumToBits(counter) ;
  if (decPt) {
    bits = bits | B00000001;  // add decimal point if needed
  }
  myfnUpdateDisplay(bits);    // display alphanumeric digit
  delay(300);                 // pause for 1/2 second

}

void myfnUpdateDisplay(byte eightBits) {
  if (common == 'a') {                  // using a common anonde display?
    eightBits = eightBits ^ B11111111;  // then flip all bits using XOR
  }
  digitalWrite(latchPin, LOW);  // prepare shift register for data
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits); // send data
  digitalWrite(latchPin, HIGH); // update display
}

byte myfnNumToBits(int someNumber) {
  switch (someNumber) {
    case 0:
      return B11111100;
      break;
    case 1:
      return B01100000;
      break;
    case 2:
      return B11011010;
      break;
    case 3:
      return B11110010;
      break;
    case 4:
      return B01100110;
      break;
    case 5:
      return B10110110;
      break;
    case 6:
      return B10111110;
      break;
    case 7:
      return B11100000;
      break;
    case 8:
      return B11111110;
      break;
    case 9:
      return B11110110;
      break;
    case 10:
      return B11101110; // Hexidecimal A
      break;
    case 11:
      return B00111110; // Hexidecimal B
      break;
    case 12:
      return B10011100; // Hexidecimal C or use for Centigrade
      break;
    case 13:
      return B01111010; // Hexidecimal D
      break;
    case 14:
      return B10011110; // Hexidecimal E
      break;
    case 15:
      return B10001110; // Hexidecimal F or use for Fahrenheit
      break;
    default:
      return B10010010; // Error condition, displays three vertical bars
      break;
  }
}
