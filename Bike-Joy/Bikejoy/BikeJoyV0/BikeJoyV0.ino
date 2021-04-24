#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "firebase.h"

INTERNET net;
FIREBASE firebase;
//SIM TRUE  internet
#define APN "internet"
#define USER ""
#define PASS ""

#define FIREBASE_URL      "bikejoys-783e6.firebaseio.com"   // Examples "Thaieasyelecxxx.firebaseio.com" Delete https:// and / of back URL 
#define FIREBASE_SECRET   "jKkBKcjnPsftozmjZfqvFzbhu9GDrXJWq5pwG6uk"
unsigned long previousMillis = 0;
const long interval = 2000;

AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}
void setup()
{
  Serial.begin(9600);
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
  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);
  int ret = firebase.connect();
  if (ret == 1)
  {
    firebase.setStr("bike/device1/deviceID", "001");
    firebase.setFloat("bike/device1/temperature", 0);
    firebase.setFloat("bike/device1/battery",0);
    firebase.setFloat("bike/device1/speed", 0);
    firebase.setFloat("bike/device1/location/latitude", 0);
    firebase.setFloat("bike/device1/location/longtitude", 0);
    firebase.setStr("bike/device1/pin", "1111");
    firebase.setBool("bike/device1/status", false);
  }
  firebase.close();
  previousMillis =  millis();
}
void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    int ret = firebase.connect();
    if (ret == 1) {
      //String jsObj = "{\"dataTemperature\":" + String(t) + ",\"dataHumidity\":" + String(h) + "}";  // Data must be JSON format
      //String addpth = firebase.pushStr("/bike", jsObj);
      //Serial.println("push : " + addpth);
      
      firebase.setFloat("bike/device1/temperature", random(100));
      firebase.setFloat("bike/device1/battery", random(100));
      firebase.setFloat("bike/device1/speed", random(100));
      firebase.setFloat("bike/device1/location/latitude", random(100));
      firebase.setFloat("bike/device1/location/longtitude", random(100));

      String Status  = firebase.get("bike/device1/status/");
      Serial.println("Get Status = " + String(Status));
      //      if (led == 1) {
      //        digitalWrite(LED, LOW);
      //        firebase.remove("ex_push");
      //      } else {
      //        digitalWrite(LED, HIGH);
      //      }
    }
    firebase.close();
    previousMillis = currentMillis;
  }
}
