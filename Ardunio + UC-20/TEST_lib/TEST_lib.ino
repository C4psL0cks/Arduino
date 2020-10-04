#include <HardwareSerial.h>
#define APN "internet"
#define USER "true"
#define PASS "true"

#define buttonPIN 4
#define ledS1PIN  18
#define ledS2PIN  5
#define ledTXPIN  17
#define ledRXPIN  16
#define simOnPIN  19
#define simRXPIN  14
#define simTXPIN  12

#define OFF HIGH
#define ON  LOW

HardwareSerial gsm(0);

unsigned long timeGSMOn = 0;
unsigned char flag_line_error = 1;
unsigned char CLI_ID = 4;

unsigned long previousMillis = 0;

int START_PIN = 4;
unsigned long previousMillis_timeout = 0;



void setup() {
  Serial.begin(9600);
  gsm.begin(9600, SERIAL_8N1, 3, 1); //3,1 rx-tx
  while (!Serial) {
    ;
  }
  delay(2000);
  Serial.println("ESP32-UC20");
  PowerOn();
  while (WaitReady()) {}
  Serial.print("GetOperator --> ");
  Serial.println(GetOperator());
  Serial.print("SignalQuality --> ");
  Serial.println(SignalQuality());

}
void loop() {

  //  if (mySerial.available()) {
  //    Serial.write(mySerial.read());
  //  }
  //  if (Serial.available()) {
  //    mySerial.write(Serial.read());
  //  }
}

String GetOperator()
{
  gsm.println(F("AT+COPS?"));
  while (!gsm.available())
  {}
  start_time_out();
  while (1)
  {
    String req = gsm.readStringUntil('\n');
    if (req.indexOf(F("+COPS")) != -1)
    {
      //+COPS: 0,0,"TRUE-H",2
      /*char comma1 = req.indexOf(F(","));
        char comma2 = req.indexOf(F(","),comma1+1);
        char comma3 = req.indexOf(F(","),comma2+1);
        String  operate_name = req.substring(comma2+2,comma3-1);
        String  acc_tech = req.substring(comma3+1);
        return(operate_name + "," + acc_tech);
      */
      return (req.substring(req.indexOf(F("\""))));
    }
    if (time_out(3000))
    {
      return (F(""));
    }

  }
  return (F(""));
}
unsigned char SignalQuality()
{
  unsigned char ret = 101;
  gsm.println(F("AT+CSQ"));
  while (!gsm.available())
  {}
  start_time_out();
  while (1)
  {
    String req = gsm.readStringUntil('\n');
    if (req.indexOf(F("+CSQ")) != -1)
    {
      ret = req.substring(req.indexOf(F(" ")) + 1, req.indexOf(F(","))).toInt();
      return (ret);
    }
    if (time_out(3000))
    {
      return (ret);
    }

  }
  return (ret);
}

void start_time_out()
{
  previousMillis_timeout = millis();
}
bool PowerOn()
{

  String req = "";
  //gsm.println(F("AT"));

  pinMode(START_PIN, OUTPUT);

  digitalWrite(START_PIN, HIGH);
  delay(1000);
  digitalWrite(START_PIN, LOW);
  delay(1000);

  /*
    while(!gsm.available())
    {
    delay(1);
    }
    start_time_out();
  */

  unsigned long pv_out = millis();

  while (1)
  {
    if (gsm.available())
    {
      req = gsm.readStringUntil('\n');
      // debug(req);
      if (req.indexOf(F("RDY")) != -1)
      {
        start_time_out();
        Serial.println("Power ON");
        return (true);
      }
      if (req.indexOf(F("POWERED DOWN")) != -1)
      {
        //start_time_out();
        Serial.println("Power OFF");
        pinMode(START_PIN, OUTPUT);
        digitalWrite(START_PIN, HIGH);
        delay(1000);
        digitalWrite(START_PIN, LOW);
        delay(1000);
      }

    }
    unsigned long current_out = millis();
    //debug("x");
    if (current_out - pv_out >= (10000))
    {
      digitalWrite(START_PIN, HIGH);
      delay(1000);
      digitalWrite(START_PIN, LOW);
      delay(1000);
      //      debug(F("Power Retry"));
      Serial.println("Power Retry");
      pv_out = current_out;
    }
  }
}

bool WaitReady()
{
  start_time_out();
  while (gsm.available())
  {
    String req = gsm.readStringUntil('\n');
    //    debug(req);
    Serial.println(req);
    if (req.indexOf(F("PB DONE")) != -1)
    {
      //      debug(F("\r\nUC20 Ready..."));
      //      debug(F("\r\nClose Echo"));
      Serial.println("\r\nUC20 Ready...");
      Serial.println("\r\nClose Echo");
      gsm.println(F("ATE0"));
      wait_ok(1000);

      return (false);
    }
    else if (req.indexOf(F("POWERED DOWN")) != -1)
    {
      digitalWrite(START_PIN, HIGH);
      delay(1000);
      digitalWrite(START_PIN, LOW);
      delay(1000);
      PowerOn();
    }
    else
    {
      return (true);
    }
    if (time_out(30000))
    {
      //      debug(F("\r\nNo response--> Please check Hardware"));
      Serial.println("\r\nNo response--> Please check Hardware");
    }
  }
  return (true);
}
bool time_out(long timeout_interval)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis_timeout >= timeout_interval)
  {
    previousMillis_timeout = currentMillis;
    return (true);
  }
  return (false);
}
bool wait_ok(long time)
{
  return wait_ok_(time, true);
}
bool wait_ok_(long time, bool ack)
{
  unsigned long previousMillis = millis();
  while (1)
  {
    String req = gsm.readStringUntil('\n');
    if (req.indexOf(F("OK")) != -1)
    {
      if (ack)
        //        debug(F("OK"));
        Serial.println("OK");
      return (1);
    }
    if (req.indexOf(F("ERROR")) != -1)
    {
      if (ack)
        //        debug(F("Error"));
        Serial.println("Error");
      return (0);
    }
    //debug(req);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= time)
    {
      previousMillis = currentMillis;
      if (ack)
        //        debug(F("Error"));
        Serial.println("Error");
      return (0);
    }

  }

}
