#include <AltSoftSerial.h>
#include "SoftwareSerial.h"
#include "TEE_UC20.h"
#include "internet.h"
#include "firebase.h"
#include "gnss.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_URL      "bikejoys-783e6.firebaseio.com"
#define FIREBASE_SECRET   "jKkBKcjnPsftozmjZfqvFzbhu9GDrXJWq5pwG6uk"
#define APN               "internet"
#define USER              "true"
#define PASS              "true"
#define UNLOCK            3
#define LEDSTATE          6
#define TEMP              5
#define GPS_STATUS        12

AltSoftSerial mySerial;
OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);

INTERNET net;
FIREBASE firebase;
GNSS gps;

unsigned long previousMillis = 0;
const long interval = 1000; //2000

boolean Status = false, state = false;
String statuslock = "";

String device = "device1";
int value = 0, battery = 0;
float correctionfactor = 6.5, vout = 0.0, vin = 0.0, R1 = 4700.0, R2 = 2200.0;

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
  pinMode(UNLOCK, OUTPUT);
  pinMode(LEDSTATE, OUTPUT);
  pinMode(GPS_STATUS, OUTPUT);
  digitalWrite(GPS_STATUS, LOW);
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
  float temperature = sensors.getTempCByIndex(0);
  
  //  Serial.println("temperature : " + String(temperature));
  
  if (temperature < 0) {
    temperature = 0;
  }
  Serial.println("temperature : " + String(temperature));

  value = analogRead(A0);
  vout = (value * correctionfactor) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  vin = correctionfactor - vin;
  battery = map(vin, 3.2, vin, 0, 100);
  
  //  Serial.println("V:" + String(vin, 2));
  //  Serial.println("battery:" + String(battery));

  String GPS_DATA = gps.GetPosition();
  String latitude = getValue(GPS_DATA, ',', 1);
  String longitude = getValue(GPS_DATA, ',', 2);

  if (latitude == "" && longitude == "") {
    Serial.println("GPS Wait.....");
    digitalWrite(GPS_STATUS, HIGH);
  } else {
    digitalWrite(GPS_STATUS, LOW);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 60 * 50) {

//      Serial.println(GPS_DATA);
//      Serial.println("latitude : " + String(latitude));
//      Serial.println("longitude : " + String(longitude));

      if (int(firebase.connect()) == 1) {
        statuslock = firebase.get("bike/" + device + "/status");
        //        Serial.println(statuslock);
        if (statuslock == "true") {
          firebase.setFloat("bike/" + device + "/battery", battery);
          delay(100);
          firebase.setFloat("bike/" + device + "/temperature", temperature);
          delay(100);
          firebase.setStr("bike/" + device + "/location/latitude", latitude);
          firebase.setStr("bike/" + device + "/location/longitude", longitude);
          delay(100);
        }
      }
      firebase.close();
      previousMillis = currentMillis;
    }
  }
  if (statuslock == "true") {
    Serial.println("No Alert");
    digitalWrite(UNLOCK, HIGH);
    digitalWrite(LEDSTATE, HIGH);
  } else if (statuslock == "false") {
    Serial.println("Alert");
    digitalWrite(UNLOCK, LOW);
    digitalWrite(LEDSTATE, LOW);
  }
  delay(200);
}
