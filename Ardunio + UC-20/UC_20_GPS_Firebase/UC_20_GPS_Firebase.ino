#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "firebase.h"
#include "gnss.h"
GNSS gps;
INTERNET net;
FIREBASE firebase;
//SIM TRUE internet
#define APN "internet"
#define USER "true"
#define PASS "true"

// Config Firebase
#define FIREBASE_URL "gsms-1efb6.firebaseio.com"
#define FIREBASE_SECRET "U0gghGOrOZzsYOdL7t8uwTgMGwErUjlfymGgqoGr"

unsigned long previousMillis = 0;
const long interval = 2000;

AltSoftSerial mySerial;

void debug(String data) {
  Serial.println(data);
}
void data_out(char data) {
  Serial.write(data);
}
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

void setup() {

  Serial.begin(9600);       //เริ่มใช้งาน Serial
  gsm.begin(&mySerial, 9600);     //กำหนดช่องทาง Serial ที่ต่อกับ 3GShield
  gsm.Event_debug = debug;      //กำหนด Function ที่ใช้แสดง Debug
  Serial.println(F("UC20"));
  gsm.PowerOn();          //สั่งงานปิด และ เปิด 3G Module
  while (gsm.WaitReady()) {}    //รอจนกว่า 3G จะเชื่อมต่อกับเครือข่ายและพร้อมใช้งาน
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());  //แสดงชื่อเครือข่ายที่ใช้
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());  //แสดงความแรงของสัญญาณ
  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS); //ตั้งค่า APN USER PASSWORD
  Serial.println(F("Connect net"));
  net.Connect();          //เชื่อมต่อ Internet
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());    //แสดง IP

  firebase.begin(FIREBASE_URL, FIREBASE_SECRET);

  gps.Start();
  Serial.print(F("GPS Start"));

  int ret = firebase.connect(); //เชื่อมต่อกับ Firebase
  firebase.close();      //ปิดการเชื่อมต่อกับ Firebase
  previousMillis =  millis();  //กำหนดเวลาปัจจุบัน ให้เป็นค่าเวลาก่อนหน้า

}
void loop() {

  unsigned long currentMillis = millis(); //อ่านค่าเวลาปัจจุบัน

  //ถ้าหากเวลาปัจจุบันลบกับเวลาก่อนหน้า มากกว่าหรือเท่ากับค่าเวลาในตัวแปล Interval (ทำงานทุกๆ 2 วินาที)
  if (currentMillis - previousMillis >= interval)
  {
    int ret = firebase.connect();    //เชื่อมต่อกับ Filebase
    if (ret == 1) {
      String GPS_DATA = gps.GetPosition();
      // Serial.println(GPS_DATA);
      String latitude = getValue(GPS_DATA, ',', 1 );
      String longitude = getValue(GPS_DATA, ',', 2 );
      Serial.println("latitude : " + latitude);
      Serial.println("longitude : " + longitude);
      firebase.setStr("latitude", latitude);
      firebase.setStr("longitude", longitude);
    }
    firebase.close();       //ปิดการเชื่อมต่อกับ Filebase
    previousMillis = currentMillis;
  }
}
