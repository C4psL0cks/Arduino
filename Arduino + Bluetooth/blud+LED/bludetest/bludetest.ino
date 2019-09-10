#include <SoftwareSerial.h>
SoftwareSerial mySerial(A0, A1); // RX, TX
int state = 0;
int LEDbluetooth = 3;

void setup() {

  Serial.begin(9600);
  while (!Serial) ;
  mySerial.begin(9600);


}
void loop() 
{

  if (mySerial.available() > 0)
  {
    char temp = mySerial.read();
    if (temp == 'a')
    {
      ledonBluetooth();
    }

    else if (temp == 'b')
    {
      ledoffBluetooth();

    }
  }
  if (Serial.available())
  {
    mySerial.write(Serial.read());
  }

}
void ledonBluetooth()
{
  digitalWrite(LEDbluetooth, HIGH);
}
void ledoffBluetooth()
{
  digitalWrite(LEDbluetooth, LOW);
}
