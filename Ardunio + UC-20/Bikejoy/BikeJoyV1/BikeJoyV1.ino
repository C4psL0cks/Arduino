#include <AltSoftSerial.h>
#include "SoftwareSerial.h"
#include "TEE_UC20.h"
#include "internet.h"
#include "firebase.h"
#include "gnss.h"

#define FIREBASE_URL      "bikejoys-783e6.firebaseio.com"
#define FIREBASE_SECRET   "jKkBKcjnPsftozmjZfqvFzbhu9GDrXJWq5pwG6uk"
#define APN "internet"
#define USER "true"
#define PASS "true"
#define UNLOCK 3
#define LEDSTATE 6

AltSoftSerial mySerial;
INTERNET net;
FIREBASE firebase;
GNSS gps;

unsigned long previousMillis = 0;
const long interval = 1000; //2000
boolean Status = false;
boolean state = false;
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

  float temperature = random(25, 40);
  int battery = 100;
  int speeds = random(0, 100);
  String GPS_DATA = gps.GetPosition();
  String latitude = getValue(GPS_DATA, ',', 1);
  String longitude = getValue(GPS_DATA, ',', 2);

  if (latitude == "" && longitude == "") {
    Serial.println("GPS Wait.....");
  } else {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // Serial.println(GPS_DATA);
      // Serial.println("latitude : " + String(latitude));
      // Serial.println("longitude : " + String(longitude));
      // int ret = firebase.connect();
      if (int(firebase.connect()) == 1) {
        Serial.println("battery : " + String(battery));
        Serial.println("temperature : " + String(temperature));
        Serial.println("latitude : " + String(latitude));
        Serial.println("longitude : " + String(longitude));
        //  String x = firebase.get("bike/device2/status");
        //  Serial.println(x);
        if (String(firebase.get("bike/device2/status")) == "true") {
          firebase.setFloat("bike/device2/battery", 100);
          firebase.setFloat("bike/device2/temperature", temperature);
          firebase.setStr("bike/device2/location/latitude", latitude);
          firebase.setStr("bike/device2/location/longitude", longitude);
          firebase.setInt("bike/device2/speed", speeds);
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
