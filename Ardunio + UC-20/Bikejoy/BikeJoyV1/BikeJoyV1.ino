#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "firebase.h"
#include "gnss.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 3;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

INTERNET net;
FIREBASE firebase;
GNSS gps;
AltSoftSerial mySerial;

#define FIREBASE_URL      "bikejoys-783e6.firebaseio.com"
#define FIREBASE_SECRET   "jKkBKcjnPsftozmjZfqvFzbhu9GDrXJWq5pwG6uk"
unsigned long previousMillis = 0;
const long interval = 2000;
boolean Status = false;

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
void debug(String data)
{
  Serial.println(data);
}
void setup()
{
  Serial.begin(9600);
  // Start the DS18B20 sensor
  sensors.begin();
  gsm.begin(&mySerial, 9600);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
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
  gps.Start();
  Serial.print(F("GPS Start"));

  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  previousMillis =  millis();

}
void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    int ret = firebase.connect();
    if (ret == 1)
    {
      float temperature = random(20, 40);
      int battery = random(20, 100);
      int speeds = random(0, 100);
      String GPS_DATA = gps.GetPosition();
      Serial.println(GPS_DATA);
      String latitude = getValue(GPS_DATA, ',', 1);
      String longitude = getValue(GPS_DATA, ',', 2);

      Serial.println("battery : " + String(battery));
      Serial.println("temperature : " + String(temperature));
      Serial.println("latitude : " + String(latitude));
      Serial.println("longitude : " + String(longitude));

      String x = firebase.get("bike/device1/status");
      Serial.println(x);
      if (x == "true") {
        firebase.setFloat("bike/device1/battery", battery);
        firebase.setFloat("bike/device1/temperature", temperature);
        firebase.setStr("bike/device1/location/latitude", latitude);
        firebase.setStr("bike/device1/location/longitude", longitude);
        firebase.setInt("bike/device1/speed", speeds);
      }
      else {
        Serial.println("Alert");
      }
    }
    firebase.close();
    previousMillis = currentMillis;
  }


}
