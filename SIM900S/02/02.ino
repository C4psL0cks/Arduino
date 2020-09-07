#include <string.h>
#include <Wire.h>

//*******for senser **********
float humidity;//humidity
float temperature;//temperature
String showString = ""; //

#define DEBUG true
int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

bool ModuleState = false;
unsigned long timeCount;

void setup()
{
  pinMode(PWR_KEY, OUTPUT);
  pinMode(RST_KEY, OUTPUT);
  pinMode(LOW_PWR_KEY, OUTPUT);
  digitalWrite(RST_KEY, LOW);
  digitalWrite(LOW_PWR_KEY, HIGH);
  digitalWrite(PWR_KEY, HIGH);

  SerialUSB.begin(115200);
  while (!SerialUSB) {
    ; // wait for serial port to connect
  }
  Serial1.begin(115200);
  SerialUSB.println("Start to run!");
  
  /**
    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(5000);
      ModuleState=moduleStateCheck();

      if(ModuleState==false)//if it's off, turn on it.
      {
          digitalWrite(PWR_KEY, LOW);
          delay(3000);
          digitalWrite(PWR_KEY, HIGH);
          delay(5000);
    SerialUSB.println("Now turnning the A9/A9G on.");
      }
  **/

  //GPS test
  //sendData("AT+GPS=1", 1000, DEBUG);//1: turn on GPS  0:Turn off GPS
  //sendData("AT+GPSRD=10", 1000, DEBUG);//Read NEMA information every 10 seconds
  //sendData("AT+LOCATION=2", 1000, DEBUG);//AT+LOCATION=X  //1:Get base address, 2:get gps address
  //sendData("AT+CCID", 3000, DEBUG);
  //sendData("AT+CREG?", 3000, DEBUG);
  //sendData("AT+CGATT=1", 1000, DEBUG);
  //sendData("AT+CGACT=1,1", 1000, DEBUG);
  //sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"", 1000, DEBUG);
  //sendData("AT+CIPSTART=\"TCP\",\"www.mirocast.com\",80", 2000, DEBUG);

  delay(500);
  timeCount = millis();
  SerialUSB.println(" A9/A9G GPS Test Begin!");
}

void loop() {

  if (millis() - timeCount > 1000) {
    humidity = 10;
    temperature = 20;
    SerialUSB.println("Read from DHT sensor!");
    showString = "Humidity: " + (String)humidity + "%" + "\r\n" + "Temp.:    " + (String)temperature + "C";

    SerialUSB.println(showString);
    timeCount = millis(); //refresh
  }

  while (Serial1.available() > 0) {
    String cstring = Serial1.readString();
    SerialUSB.print(cstring);//SerialUSB.write(Serial1.read());
    yield();
  }

  while (SerialUSB.available() > 0) {
    Serial1.write(SerialUSB.read());
    yield();
  }
}

bool moduleStateCheck()
{
  int i = 0;
  bool moduleState = false;
  for (i = 0; i < 10; i++)
  {
    String msg = String("");
    msg = sendData("AT", 1000, DEBUG);
    if (msg.indexOf("OK") >= 0)
    {
      SerialUSB.println("A9/A9G Module had turned on.");
      moduleState = true;
      return moduleState;
    }
    delay(500);
  }
  return moduleState;
}

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }
  if (debug)
  {
    SerialUSB.print(response);
  }
  return response;
}
