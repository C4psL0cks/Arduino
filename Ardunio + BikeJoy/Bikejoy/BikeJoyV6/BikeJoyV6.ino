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

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
unsigned long previousMillis5 = 0;
unsigned long previousMillis6 = 0;
const long interval = 1000; //2000
int delays = 200;
int setinterval = 50;
float temperature = 0;

String latitude = "";
String longitude = "";
String states = "";
boolean around_state = false;
boolean state = false;
boolean runs = false;
String sos = "";

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
  delay(500);

  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);

  if (int(firebase.connect()) == 1) {
    Serial.println("Create setinterval");
    setinterval = firebase.getInt("bike/setinterval/");
    delays      = firebase.getInt("bike/delays/");
    state  = firebase.getInt("bike/" + device + "/status");
    delay(100);
  }
  firebase.close();

  previousMillis1 = millis();
  previousMillis2 = millis();
  previousMillis3 = millis();
  previousMillis4 = millis();
  previousMillis5 = millis();
  previousMillis6 = millis();

}
void loop() {

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  if (temperature < 0) {
    temperature = 0;
  }

  value = analogRead(A0);
  vout = (value * correctionfactor) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  vin = correctionfactor - vin;
  //  battery = map(vin, 3.0 , vin, 0, 100);

  String GPS_DATA = gps.GetPosition();
  latitude = getValue(GPS_DATA, ',', 1);
  longitude = getValue(GPS_DATA, ',', 2);

  if (latitude == "" && longitude == "") {
    digitalWrite(GPS_STATUS, HIGH);
    runs = false;
    Serial.println("-------------------------------------");
    Serial.println("GPS Wait.....");
    Serial.println("-------------------------------------");

  } else if (latitude != "" && longitude != "") {
    digitalWrite(GPS_STATUS, LOW);
    runs = true;
    Serial.println("-------------------------------------");
    Serial.println("GPS Start....");
    Serial.println("temperature : " + String(temperature));
    Serial.println("V:" + String(vin, 2));
    Serial.println("battery:" + String(battery));
    Serial.println("runs:" + String(runs));
    Serial.println("setinterval:" + String(setinterval));
    Serial.println("delays:" + String(delays));
    Serial.println("state:" + String(state));
    Serial.println("-------------------------------------");
  }

  if (int(firebase.connect()) == 1) {
    states = firebase.get("bike/" + device + "/status/");
    // delay(100);
    // around_state = false;
  }
  firebase.close();

  // if (runs == true && around_state == false) {
  //   around_state = true;
  //   unsigned long currentMillis1 = millis();
  //   if (currentMillis1 - previousMillis1 >= 60 * setinterval) {
     
  //     previousMillis1 = currentMillis1;
  //   }
  // }

  if ((runs == "true") && (states == "true") && (around_state == false)) {
    around_state = true;
    unsigned long currentMillis2 = millis();
    if (currentMillis2 - previousMillis2 >= 60 * setinterval) {
      if (int(firebase.connect()) == 1) {
        firebase.setStr("bike/" + device + "/location/latitude", latitude);
        firebase.setStr("bike/" + device + "/location/longitude", longitude);
        delay(50);
        firebase.setInt("bike/" + device + "/battery", battery);
        firebase.setFloat("bike/" + device + "/temperature", temperature);
        delay(50);
        around_state = false;
      }
      firebase.close();
      previousMillis2 = currentMillis2;
    }
  }

  if (runs == "true" && (states == "true") && around_state == false) {
    around_state = true;
    unsigned long currentMillis3 = millis();
    if (currentMillis3 - previousMillis3 >= 60 * setinterval) {
      if (int(firebase.connect()) == 1) {
        sos = firebase.get("bike/" + device + "/sos/");
        delay(100);
        around_state = false;
      }
      firebase.close();
      previousMillis3 = currentMillis3;
    }
  }

  if (states == "true") {
    Serial.println("No Alert");
    digitalWrite(UNLOCK, HIGH);
    digitalWrite(LEDSTATE, HIGH);
  } else if (states == "false") {
    Serial.println("Alert");
    digitalWrite(UNLOCK, LOW);
    digitalWrite(LEDSTATE, LOW);
  }

  if (sos == "true") {
    Serial.println("Alert");
    digitalWrite(UNLOCK, LOW);
    digitalWrite(LEDSTATE, LOW);

  } else if (sos == "false") {
    Serial.println("No Alert");
    digitalWrite(UNLOCK, HIGH);
    digitalWrite(LEDSTATE, HIGH);
  }
  delay(delays);
}
