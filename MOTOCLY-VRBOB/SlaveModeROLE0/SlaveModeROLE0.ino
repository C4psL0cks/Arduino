//HM-10 Setup
#include <AltSoftSerial.h>
AltSoftSerial bluetooth; // tx9 rx8

String inText;
void setup() {
  Serial.begin(9600);  // Debug Serial
  while (!Serial);
  bluetooth.begin(9600);   // to AltSoftSerial RX


  // connect to the remote Bluetooth module
  bluetooth.println("AT+IMME1");
  delay(1000);
  bluetooth.println("AT+ROLE0");
  delay(1000);
  bluetooth.println("AT+CONA50338BF1F66D");
  delay(1000);

  Serial.println("AltSoftSerial");
}


void loop() {

  Serial.flush();

  if (bluetooth.available()) {
    Serial.write(bluetooth.read());
  }

  //  inText = ""; //clears variable for new input
  //  while (bluetooth.available())  {
  //    char c = bluetooth.read();  //gets one byte from serial buffer
  //    Serial.println(c);
  //    //    delay(5);
  //
  //    if (c != '_') { // div
  //      inText += c;
  //      //      delay(5);
  //    }
  //    else {
  //      if (c == '#') { // end
  //        break;
  //      }
  //    }
  //    Serial.println(inText);
  //  }

  bluetooth.flush();
}
