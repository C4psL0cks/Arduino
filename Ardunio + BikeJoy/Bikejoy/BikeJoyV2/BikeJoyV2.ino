#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "firebase.h"
#include "gnss.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"
#define TEMPPIN 3
#define VOLTAGEPIN A0
#define LEDSTATEGPS 7

#define FIREBASE_URL      "bikejoys-783e6.firebaseio.com"
#define FIREBASE_SECRET   "jKkBKcjnPsftozmjZfqvFzbhu9GDrXJWq5pwG6uk"

// Value
String IP;
String GPS_DATA;
String latitude;
String longitude;
float temperature;
float battery;
int speeds;

unsigned long previousMillis = 0;
const long interval = 1000; //2000
boolean Status = false;

INTERNET net;
FIREBASE firebase;
GNSS gps;

AltSoftSerial mySerial;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(TEMPPIN);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

String getValue(String data, char separator, int index) {
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
void debug(String data) {
  Serial.println(data);
}
void setup() {
  Serial.begin(9600);
  pinMode(VOLTAGEPIN, INPUT);
  pinMode(LEDSTATEGPS, OUTPUT);
  digitalWrite(LEDSTATEGPS, LOW);
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
  IP = net.GetIP();
  Serial.println(IP);
  gps.Start();
  Serial.println(F("GPS Start"));

  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  previousMillis =  millis();

}
void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    int ret = firebase.connect();
    if (ret == 1) {
      String BIKE_STATE = firebase.get("bike/device1/status");
      // Temperature
      sensors.requestTemperatures();
      temperature = sensors.getTempCByIndex(0);

      // Battery
      battery = ((analogRead(VOLTAGEPIN) * 5.14) / 1024.0) / (2200.0 / (4700.0 + 2200.0));
      // Serial.println("INPUT V:" + String(battery, 2));

      // Speed
      speeds = random(0, 100);

      // GPS
      GPS_DATA = gps.GetPosition();
      Serial.println(GPS_DATA);

      //+QGPSLOC: 112431.0,
      //16,44.640884,N,dd.mmmmmmmm/n/s
      //100,11.822662,E,dd.mmmmmmmm/n/s
      //0.9,56.1,2,234.11,0.0,0.0,101220,08
      //
      //16.743107, dd.dddddd
      //100.197038, dd.dddddd

      latitude = getValue(GPS_DATA, ',', 1);
      longitude = getValue(GPS_DATA, ',', 2);
      //      if (latitude == 0 && longitude == 0) {
      //        Serial.println("GPS OFF");
      //        digitalWrite(LEDSTATEGPS, LOW);
      //      }
      //      else {
      //        Serial.println("GPS ON");
      //        digitalWrite(LEDSTATEGPS, HIGH);
      //      }
      Serial.println("SEND DATA TO DATABASE.....");
      Serial.println("..............................");
      Serial.println("battery : " + String(battery));
      Serial.println("temperature : " + String(temperature));
      Serial.println("latitude : " + String(latitude));
      Serial.println("longitude : " + String(longitude));
      Serial.println("..............................");
      //      delay(50);
      // Serial.println("STATE BIKE ON:" + BIKE_STATE);
      if (BIKE_STATE == "true") {
        firebase.setStr("bike/device1/ip", IP);
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
