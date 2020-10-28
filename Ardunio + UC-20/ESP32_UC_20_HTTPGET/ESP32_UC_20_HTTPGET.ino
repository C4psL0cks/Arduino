#include <HardwareSerial.h>
#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "http.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define simRXPIN  16//14
#define simTXPIN  17//12

INTERNET net;
UC_FILE file;
HTTP http;

HardwareSerial mySerial(2);

void debug(String data)
{
  Serial.println(data);
}
void data_out(char data)
{
  Serial.write(data);
}
void setup()
{
  Serial.begin(9600);
  gsm.begin(&mySerial, 9600, simRXPIN, simTXPIN);
  gsm.Event_debug = debug;
  Serial.println(F("ESP32-UC20"));
  //gsm.PowerOn();
  while (gsm.WaitReady()) {}
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());

  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());
  Serial.println(F("Start HTTP"));
  http.begin(1);
  Serial.println(F("Send HTTP GET"));
  http.url("https://postman-echo.com/get?foo1=bar1&foo2=bar2");
  Serial.println(http.get());
  Serial.println(F("Clear data in RAM"));
  file.Delete(RAM, "*");
  Serial.println(F("Save HTTP Response To RAM"));
  http.SaveResponseToMemory(RAM, "web.hml");
  Serial.println(F("Read data in RAM"));
  read_file(RAM, "web.hml");
  Serial.println(F("Disconnect net"));
  net.DisConnect();
}

void read_file(String pattern, String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern, file_name);
}
void loop()
{
  while (gsm.available()) {
    Serial.write(gsm.read());
  }
  while (Serial.available()) {
    gsm.write(Serial.read());
  }

  //  http
  //  if (gsm.available())
  //  {
  //    Serial.write(gsm.read());
  //  }
  //  if (Serial.available())
  //  {
  //    char c = Serial.read();
  //    gsm.write(c);
  //  }
}
