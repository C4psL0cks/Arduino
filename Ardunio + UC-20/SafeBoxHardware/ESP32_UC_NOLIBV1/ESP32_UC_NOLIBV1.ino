#include <HardwareSerial.h>

#define APN "internet"
#define USER "true"
#define PASS "true"

#define simRXPIN  14
#define simTXPIN  12

HardwareSerial MySerial(2);

int START_PIN = 4;
unsigned long previousMillis_timeout = 0;

void debug(String data) {
  Serial.println(data);
}

void start_time_out()
{
  previousMillis_timeout = millis();
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

bool PowerOn() {
  String req = "";
  unsigned long pv_out = millis();

  while (1)
  {
    if (MySerial.available())
    {
      req = MySerial.readStringUntil('\n');
      // debug(req);
      if (req.indexOf("RDY") != -1)
      {
        start_time_out();
        debug("Power ON");
        return (true);
      }
      if (req.indexOf("POWERED DOWN") != -1)
      {
        //start_time_out();
        debug("Power OFF");
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
      debug("Power Retry");
      pv_out = current_out;
    }
  }
}

bool WaitReady()
{
  start_time_out();
  while (MySerial.available())
  {
    String req = MySerial.readStringUntil('\n');
    debug(req);
    if (req.indexOf(F("PB DONE")) != -1)
    {
      debug(F("\r\nUC20 Ready..."));
      debug(F("\r\nClose Echo"));
      MySerial.print("ATE0\r");
      wait_ok(10000);

      return (false);
    }
    else if (req.indexOf("POWERED DOWN") != -1)
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
      debug(F("\r\nNo response--> Please check Hardware"));
    }
  }
  return (true);
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
    String req = MySerial.readStringUntil('\n');
    if (req.indexOf(F("OK")) != -1)
    {
      if (ack)
        Serial.println("OK");
      return (1);
    }
    if (req.indexOf(F("ERROR")) != -1)
    {
      if (ack)
        Serial.println("Error");
      return (0);
    }
    debug(req);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= time)
    {
      previousMillis = currentMillis;
      if (ack)
        Serial.println("Error");
      return (0);
    }
  }
}

void setup() {

  Serial.begin(9600);
  MySerial.begin(9600, SERIAL_8N1, simRXPIN, simTXPIN); //3,1 rx-tx
  while (WaitReady()) {}
  Serial.println(F("Set APN and Password"));
  netConfigure(APN, USER, PASS);
  wait_ok(10000);
  Serial.println(F("Connect net"));
  netConnect();
  wait_ok(10000);
  Serial.println("Initializing...");
  delay(1000);
}

void loop() {

  if (MySerial.available()) {
    Serial.write(MySerial.read());
  }
  if (Serial.available()) {
    MySerial.write(Serial.read());
  }
}

// INTER NET
bool netConfigure(String apn, String user, String password)
{
  String  str = "AT+QICSGP=1,1,";
  str += "\"" + apn + "\",";
  str += "\"" + user + "\",";
  str += "\"" + password + "\",";
  str += "1";
  MySerial.println(str);
  return (wait_ok(10000));
}

bool netConnect()
{
  String str = "AT+QIACT=1";
  MySerial.println(str);
  return (wait_ok(10000));
}

bool DisConnect()
{
  MySerial.println("AT+QIDEACT=1");
  return (wait_ok(10000));
}
