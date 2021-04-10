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
//#define LEDSTATE          6
#define TEMP              5
#define GPS_STATUS        12

AltSoftSerial mySerial;
OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);

INTERNET net;
FIREBASE firebase;
GNSS gps;

unsigned long previousMillis = 0;
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
  pinMode(GPS_STATUS, OUTPUT);
  digitalWrite(GPS_STATUS, HIGH);
  digitalWrite(UNLOCK, HIGH);

  gsm.begin(&mySerial, 9600);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}
  //  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  //  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());
  //  Serial.println(F("Disconnect net"));
  net.DisConnect();
  //  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS);
  //  Serial.println(F("Connect net"));
  net.Connect();
  //  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());

  String ip = net.GetIP();
  if (ip != "") {
    digitalWrite(GPS_STATUS, HIGH);
    delay(500);
    digitalWrite(GPS_STATUS, LOW);
    delay(500);
    digitalWrite(GPS_STATUS, HIGH);
    delay(500);
  }

  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  if (int(firebase.connect()) == 1) {
    //    Serial.println(F("Firebase Start"));
    setinterval = firebase.getInt("bike/setinterval/");
    delay(100);
  }
  firebase.close();
  previousMillis = millis();

  gps.Start();
  //  Serial.println(F("GPS Start"));
  delay(100);
}
void loop() {


  if (int(firebase.connect()) == 1) {
    states = firebase.get("bike/" + device + "/status/");
    setinterval = firebase.getInt("bike/setinterval/");
  }
  firebase.close();
  //
  //  Serial.println("-------------------------------------");
  //  Serial.println("STATE : " + String(states));
  //  Serial.println("Setinterval : " + String(setinterval));
  //  Serial.println("battery : " + String(battery));
  //  Serial.println("-------------------------------------");

  if (states == "true") {
    //    Serial.println("No Alert");
    digitalWrite(UNLOCK, HIGH);
  } else if (states == "false") {
    //    Serial.println("Alert");
    digitalWrite(UNLOCK, LOW);
  }

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

  unsigned long currentMillis = millis();
  String GPS_DATA = gps.GetPosition();
  if (GPS_DATA.indexOf(F("+QGPSLOC")) != -1) { // if detected gnss data
    digitalWrite(GPS_STATUS, LOW);
    String latitude = getValue(GPS_DATA, ',', 1 );
    String longitude = getValue(GPS_DATA, ',', 2 );

    //    Serial.println("-------------------------------------");
    //    Serial.println("GPS Start....");
    //    Serial.println("latitude : " + String(latitude));
    //    Serial.println("longitude : " + String(longitude));
    //    Serial.println("-------------------------------------");

    if (currentMillis - previousMillis >= 60 * setinterval)
    {
      int ret = firebase.connect();
      if (states == "true" && ret == 1)
      {
        firebase.setStr("bike/" + device + "/location/latitude", latitude);
        firebase.setStr("bike/" + device + "/location/longitude", longitude);
        firebase.setStr("bike/" + device + "/battery", String(battery));
        firebase.setStr("bike/" + device + "/temperature", String(temperature));
      }
      firebase.close();
      previousMillis = currentMillis;
    }
  }
}
