#include <AltSoftSerial.h>
#include "SoftwareSerial.h"
#include "TEE_UC20.h"
#include "internet.h"
#include "firebase.h"
#include "gnss.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BMI160Gen.h>

#define FIREBASE_URL      "bikejoys-783e6.firebaseio.com"
#define FIREBASE_SECRET   "jKkBKcjnPsftozmjZfqvFzbhu9GDrXJWq5pwG6uk"
#define APN "internet"
#define USER "true"
#define PASS "true"
#define UNLOCK 3
#define LEDSTATE 6
#define oneWireBus 5

AltSoftSerial mySerial;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
INTERNET net;
FIREBASE firebase;
GNSS gps;

unsigned long previousMillis = 0;
const long interval = 1000; //2000
boolean Status = false;
boolean state = false;
int value = 0;
float correctionfactor = 5.0;
float vout = 0.0;
float vin = 0.0;
float R1 = 4700.0; //4.7 k
float R2 = 2200.0; //2.2 k


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
  sensors.begin();
  pinMode(UNLOCK, OUTPUT);
  pinMode(LEDSTATE, OUTPUT);
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
  Serial.println(F("GPS Start"));
  delay(100);
  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  previousMillis = millis();

}
void loop() {

  sensors.requestTemperatures();

  unsigned long currentMillis = millis();
  float temperature = sensors.getTempCByIndex(0);
  String GPS_DATA = gps.GetPosition();

  //  value = analogRead(A0);
  //  vout = (value * correctionfactor) / 1024.0;
  //  vin = vout / (R2 / (R1 + R2));
  //  Serial.println("INPUT :" + String(vin));

  if (GPS_DATA.indexOf(F("+QGPSLOC")) != -1) {

    if (currentMillis - previousMillis >= interval) {
      // Serial.println(GPS_DATA);
      String latitude = getValue(GPS_DATA, ',', 1 );
      String longitude = getValue(GPS_DATA, ',', 2 );
      // Serial.println("latitude : " + String(latitude));
      // Serial.println("longitude : " + String(longitude));

      if (int(firebase.connect()) == 1) {
        //  Serial.println("battery : " + String(battery));
        //  Serial.println("temperature : " + String(temperature));
        //  Serial.println("latitude : " + String(latitude));
        //  Serial.println("longitude : " + String(longitude));
        //  String x = firebase.get("bike/device2/status");
        //  Serial.println(x);
        if (String(firebase.get("bike/device2/status")) == "true") {
          firebase.setFloat("bike/device2/battery", vin);
          firebase.setFloat("bike/device2/temperature", temperature);
          firebase.setStr("bike/device2/location/latitude", latitude);
          firebase.setStr("bike/device2/location/longitude", longitude);
          state = true;
        }
        else {
          state = false;
        }
      }
      firebase.close();
      previousMillis = currentMillis;
    }
  }
  if (state) {
    Serial.println("No Alert");
    digitalWrite(UNLOCK, HIGH);
    digitalWrite(LEDSTATE, HIGH);
  } else {
    Serial.println("Alert");
    digitalWrite(UNLOCK, LOW);
    digitalWrite(LEDSTATE, LOW);
  }
  delay(500);
}
