#include "SPI.h"
#include "Ethernet.h"
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");

  // start the Ethernet connection:
  while (Ethernet.begin(mac) != 1){
    Serial.print(".");
  }
  Serial.print("My IP :");
  Serial.println(Ethernet.localIP());
}

void loop(){

}
