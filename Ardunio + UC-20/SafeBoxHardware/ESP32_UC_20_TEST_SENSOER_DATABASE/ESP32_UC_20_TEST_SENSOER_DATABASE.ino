#include <HardwareSerial.h>
#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "http.h"
#include "gnss.h"
#include <WiFi.h>

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define simRXPIN  14
#define simTXPIN  12

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
#define oneWireBus 18
#define water1 26
#define water2 27
#define motion 25
#define car 33

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

int water_sensor_1 = 0;
int water_sensor_2 = 0;
int motion_sensor = 0;
int car_sensor = 0;
float temperatureC = 0;
String mac_address = "";


INTERNET net;
UC_FILE file;
HTTP http;
GNSS gps;

HardwareSerial mySerial(2);

void debug(String data)
{
  Serial.println(data);
}
void data_out(char data)
{
  Serial.write(data);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup()
{
  Serial.begin(9600);
  //  sensors.begin();
  pinMode(water1, INPUT);
  pinMode(water2, INPUT);
  pinMode(motion, INPUT);
  pinMode(car, INPUT);
  gsm.begin(&mySerial, 9600, simRXPIN, simTXPIN);
  gsm.Event_debug = debug;
  Serial.println(F("ESP32-UC20"));
  gsm.PowerOn();
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
  //  gps.Start();
  //  Serial.print(F("GPS Start"));

  Serial.print("ESP Board MAC Address:  ");
  mac_address = WiFi.macAddress();
  Serial.println(mac_address);
}

void read_file(String pattern, String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern, file_name);
}
void loop()
{

  //  if (gps.GetPosition() == "") {
  //    Serial.println("Wait GPS......");
  //  }
  //  else {
  //    String GPS_DATA = gps.GetPosition();
  //    Serial.println(GPS_DATA);
  //    String latitude = getValue(GPS_DATA, ',', 1 );
  //    Serial.println("latitude : " + latitude);
  //    String longitude = getValue(GPS_DATA, ',', 2 );
  //    Serial.println("longitude : " + longitude);
  //    water_sensor_1 = digitalRead(water1);
  //    Serial.println("Water1:" + String(water_sensor_1));
  //    water_sensor_2 = digitalRead(water2);
  //    Serial.println("Water2:" + String(water_sensor_2));
  //    motion_sensor = digitalRead(motion);
  //    Serial.println("motion:" + String(motion_sensor));
  //    car_sensor = digitalRead(car);
  //    Serial.println("car:" + String(car_sensor));
  //    sensors.requestTemperatures();
  //    temperatureC = sensors.getTempCByIndex(0);
  //    Serial.println(String(temperatureC) + "ºC");
  //    Serial.println("---------------------------------------------");
  //
  //    Serial.println(F("Start HTTP"));
  //    http.begin(1);
  //    Serial.println(F("Send HTTP GET"));
  //    // http.url("https://postman-echo.com/get?latitude=" + latitude
  //    //          + "&longitude=" + longitude
  //    //          + "&water_sensor_1=" + water_sensor_1
  //    //          + "&water_sensor_2=" + water_sensor_2
  //    //          + "&motion_sensor=" + motion_sensor
  //    //          + "&car_sensor=" + car_sensor
  //    //          + "&temperatureC=" + temperatureC);
  //    http.url("http://103.233.193.144:5001/get?macaddress_uaa=" + mac_address
  //             + "&latitude=" + latitude
  //             + "&longitude=" + longitude
  //             + "&water_sensor_1=" + water_sensor_1
  //             + "&water_sensor_2=" + water_sensor_2
  //             + "&motion_sensor=" + motion_sensor
  //             + "&car_sensor=" + car_sensor
  //             + "&temperatureC=" + temperatureC);
  //    Serial.println(http.get());
  //    Serial.println();
  //    Serial.println(F("Clear data in RAM"));
  //    file.Delete(RAM, "*");
  //    Serial.println(F("Save HTTP Response To RAM"));
  //    http.SaveResponseToMemory(RAM, "web.hml");
  //    Serial.println(F("Read data in RAM"));
  //    read_file(RAM, "web.hml");
  //    Serial.println(F("Disconnect net"));
  //    net.DisConnect();
  //
  //  }
  //
  //  delay(1000);

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
