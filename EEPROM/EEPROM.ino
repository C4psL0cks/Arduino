
#include <EEPROM.h>

struct MyObject {
  float field1;
  byte field2;
  char name[10];
};

float f = 123.456f;  //Variable to store in EEPROM.
int eeAddress = 0;   //Location we want the data to be put. 255
int x = 0;

int a = 0;
int value;


void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  x = 55;
  EEPROM.put(eeAddress, x);
  EEPROM.end();
  Serial.println("Written");


}

void loop() {
  /* Empty loop */

  //  EEPROM.get(eeAddress, x);
  //  EEPROM.end();
  //  Serial.println(x);
  
  value = EEPROM.read(512);

  Serial.print(a);
  Serial.print("\t");
  Serial.print(value);
  Serial.println();

//  a = a + 1;
//
//  if (a == 512)
//    a = 0;

  delay(500);
}
