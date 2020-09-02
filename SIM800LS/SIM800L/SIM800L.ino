#include <SoftwareSerial.h>
static const int SimRXPin = 3, SimTXPin = 2;
SoftwareSerial SIM(SimTXPin, SimRXPin);

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  SIM.begin(9600);

  Serial.println("Initializing...");
  delay(500);

  SIM.println("AT"); //Once the handshake test is successful, it will back to OK
  up();
  SIM.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  up();
  SIM.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  up();
  SIM.println("AT+CREG?"); //Check whether it has registered in the network
  up();
  //  SIM.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  //  delay(500);
  //  SIM.println("AT+CSTT=internet,\"true\",\"true\""); //Check whether it has registered in the network
  //  delay(500);
  //  SIM.println("AT+CSTT?"); //Check whether it has registered in the network
  //  delay(500);
}

void loop()
{
  up();
}
void up() {
  delay(500);
  while (Serial.available())
  {
    SIM.write(Serial.read());
  }
  while (SIM.available())
  {
    Serial.write(SIM.read());
  }
}
