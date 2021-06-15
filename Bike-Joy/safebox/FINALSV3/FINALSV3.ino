#include <HardwareSerial.h>
#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "http.h"
#include "firebase.h"
#include "gnss.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//#define APN "internet"
//#define USER "true"
//#define PASS "true"

//#define FIREBASE_URL      "gsms-1efb6.firebaseio.com"
//#define FIREBASE_SECRET   "ZM8pIneWA7rbziI7J3kmyFelelZEEZ98hmgDfVjD"

//#define FIREBASE_URL      "savebox-test.firebaseio.com"
//#define FIREBASE_SECRET   "msswI7q0Znn76a391rINWLRjmtsE7bf9CK7euTWj"

#define EEPROM_SIZE 2
#define RXPIN  27
#define TXPIN  26
#define AS 21
#define AR 13
#define oneWireBus 32
#define water1 34
#define water2 35
#define motion 25
#define car 33
#define buzzer 17

INTERNET net;
UC_FILE file;
HTTP http;
GNSS gps;
FIREBASE firebase;

AsyncWebServer server(80);
HardwareSerial mySerial(2);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

int setups = 0;
int mic = 0;
int state = 0;
int water_sensor_1 = 0;
int motion_sensor = 0;
int car_sensor = 0;
int micrecord = 0;
float temperatureC = 0;
String mac_address_cut = "";
String mac_address = "";
String latitude = "-";
String longitude = "-";
boolean aroundstate = false;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
unsigned long previousMillis5 = 0;

const long interval1 = 1000UL * 60 * 3; // 3 minute interval
const long intervals1 = 5000; // 3 minute interval
const long interval2 = 1000UL * 60 * 3; // 3 minute interval
const long intervals2 = 5000; // 3 minute interval
const long interval3 = 1000UL * 60 * 3; // 3 minute interval
const long intervals3 = 5000; // 3 minute interval
const long interval4 = 1000UL * 60 * 3; // 3 minute interval
const long intervals4 = 5000; // 3 minute interval
const long interval5 = 1000UL * 60 * 3; // 3 minute interval
const long intervals5 = 5000; // 3 minute interval

boolean runs = false;
int setinterval = 3;
int cbtemp = 20;
int cbmotion = 60;
int count = 0;

const char* ssid     = "Safe-Box";
const char* password = "1234567890";

String Version = "1.0.0";
String SerialNumber = "00-00-00";
String SafeBoxName = "Safe-Box";
String SafeBoxPassword = "1234567890";
String SafeBoxFIREBASE_URL = "savebox-test.firebaseio.com";
String SafeBoxFIREBASE_SECRET = "msswI7q0Znn76a391rINWLRjmtsE7bf9CK7euTWj";
String SafeBoxDelay = "200";
String SafeBoxFTPHost = "files.000webhost.com";
String SafeBoxFTPUsername = "testerslogers";
String SafeBoxFTPPassword = "testerslogers";
String SafeBoxFTPPort = "21";
String SafeBoxAPN = "internet";
String SafeBoxUSER = "true";
String SafeBoxPASS = "true";

const char* PARAM_Version = "Version";
const char* PARAM_SerialNumber = "SerialNumber";
const char* PARAM_SafeBoxName = "SafeBoxName";
const char* PARAM_SafeBoxPassword = "SafeBoxPassword";
const char* PARAM_SafeBoxFIREBASE_URL = "SafeBoxFIREBASE_URL";
const char* PARAM_SafeBoxFIREBASE_SECRET = "SafeBoxFIREBASE_SECRET";
const char* PARAM_SafeBoxDelay = "SafeBoxDelay";
const char* PARAM_SafeBoxFTP_Host = "SafeBoxFTPHost";
const char* PARAM_SafeBoxFTP_Username = "SafeBoxFTPUsername";
const char* PARAM_SafeBoxFTP_Password = "SafeBoxFTPPassword";
const char* PARAM_SafeBoxFTP_Port = "SafeBoxFTPPort";
const char* PARAM_SafeBoxAPN = "SafeBoxAPN";
const char* PARAM_SafeBoxUSER = "SafeBoxUSER";
const char* PARAM_SafeBoxPASS = "SafeBoxPASS";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
   <title>Safe Box Hardware</title>
   <meta name="viewport" content="width=device-width, initial-scale=1">
   <script>
       function submitMessage() {
           alert("Saved value to ESP SPIFFS");
           setTimeout(function() {
               document.location.reload(false);
           }, 500);
       }
   </script>
</head>

<body>
   <h2>Safe Box Hardware</h2>
   <form action="/get" target="hidden-form">
       Version<br><input type="text" placeholder="Version" name="Version"> <br>(current value %Version%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SerialNumber<br><input type="text" placeholder="SerialNumber" name="SerialNumber"> <br>(current value %SerialNumber%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form>
   <h2>Safe Box Authorization</h2>
   <form action="/get" target="hidden-form">
       SafeBoxName<br><input type="text" placeholder="SafeBoxName" name="SafeBoxName"> <br>(current value %SafeBoxName%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBoxPassword<br><input type="text" placeholder="SafeBoxPassword" name="SafeBoxPassword"> <br>(current value %SafeBoxPassword%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form>
   <h2>Safe Box URL</h2>
   <form action="/get" target="hidden-form">
       SafeBox URL 1 <br><input type="text" placeholder="SafeBoxURL1" name="SafeBoxFIREBASE_URL"> <br>(current value %SafeBoxFIREBASE_URL%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox URL 2 <br><input type="text" placeholder="SafeBoxURL2" name="SafeBoxFIREBASE_SECRET"> <br>(current value %SafeBoxFIREBASE_SECRET%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox Delay <br><input type="text" placeholder="SafeBoxDelay" name="SafeBoxDelay"> <br>(current value %SafeBoxDelay%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form>
   <h2>Safe Box FTP</h2>
   <form action="/get" target="hidden-form">
       SafeBox FTP Host <br><input type="text" placeholder="SafeBoxFTPHost" name="SafeBoxFTPHost"> <br>(current value %SafeBoxFTPHost%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox FTP Username <br><input type="text" placeholder="SafeBoxFTPUsername" name="SafeBoxFTPUsername"> <br>(current value %SafeBoxFTPUsername%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox FTP Password <br><input type="text" placeholder="SafeBoxFTPPassword" name="SafeBoxFTPPassword"> <br>(current value %SafeBoxFTPPassword%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox FTP Port <br><input type="text" placeholder="SafeBoxFTPPort" name="SafeBoxFTPPort"> <br>(current value %SafeBoxFTPPort%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form>
    <h2>Safe Box APN</h2>
   <form action="/get" target="hidden-form">
       SafeBox APN <br><input type="text" placeholder="SafeBoxAPN" name="SafeBoxAPN"> <br>(current value %SafeBoxAPN%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox APN USER <br><input type="text" placeholder="SafeBoxUSER" name="SafeBoxUSER"> <br>(current value %SafeBoxUSER%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox APN PASS <br><input type="text" placeholder="SafeBoxPASS" name="SafeBoxPASS"> <br>(current value %SafeBoxPASS%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form>

   <iframe style="display:none" name="hidden-form"></iframe>
</body>

</html>
)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path) {
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    //    Serial.println("- empty file or failed to open file");
    return String();
  }
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, "w");
  if (!file) {
    //    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

String processor(const String& var) {
  if (var == "Version") {
    return readFile(SPIFFS, "/Version.txt");
  }
  else if (var == "SerialNumber") {
    return readFile(SPIFFS, "/SerialNumber.txt");
  }
  else if (var == "SafeBoxName") {
    return readFile(SPIFFS, "/SafeBoxName.txt");
  }
  else if (var == "SafeBoxPassword") {
    return readFile(SPIFFS, "/SafeBoxPassword.txt");
  }
  else if (var == "SafeBoxFIREBASE_URL") {
    return readFile(SPIFFS, "/SafeBoxFIREBASE_URL.txt");
  }
  else if (var == "SafeBoxFIREBASE_SECRET") {
    return readFile(SPIFFS, "/SafeBoxFIREBASE_SECRET.txt");
  }
  else if (var == "SafeBoxDelay") {
    return readFile(SPIFFS, "/SafeBoxDelay.txt");
  }
  else if (var == "SafeBoxFTPHost") {
    return readFile(SPIFFS, "/SafeBoxFTPHost.txt");
  }
  else if (var == "SafeBoxFTPUsername") {
    return readFile(SPIFFS, "/SafeBoxFTPUsername.txt");
  }
  else if (var == "SafeBoxFTPPassword") {
    return readFile(SPIFFS, "/SafeBoxFTPPassword.txt");
  }
  else if (var == "SafeBoxFTPPort") {
    return readFile(SPIFFS, "/SafeBoxFTPPort.txt");
  }
  else if (var == "SafeBoxAPN") {
    return readFile(SPIFFS, "/SafeBoxAPN.txt");
  }
  else if (var == "SafeBoxUSER") {
    return readFile(SPIFFS, "/SafeBoxUSER.txt");
  }
  else if (var == "SafeBoxPASS") {
    return readFile(SPIFFS, "/SafeBoxPASS.txt");
  }
  return String();
}

void APconifg() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    if (request->hasParam(PARAM_Version)) {
      inputMessage = request->getParam(PARAM_Version)->value();
      writeFile(SPIFFS, "/Version.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SerialNumber)) {
      inputMessage = request->getParam(PARAM_SerialNumber)->value();
      writeFile(SPIFFS, "/SerialNumber.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxName)) {
      inputMessage = request->getParam(PARAM_SafeBoxName)->value();
      writeFile(SPIFFS, "/SafeBoxName.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxPassword)) {
      inputMessage = request->getParam(PARAM_SafeBoxPassword)->value();
      writeFile(SPIFFS, "/SafeBoxPassword.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxFIREBASE_URL)) {
      inputMessage = request->getParam(PARAM_SafeBoxFIREBASE_URL)->value();
      writeFile(SPIFFS, "/SafeBoxFIREBASE_URL.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxFIREBASE_SECRET)) {
      inputMessage = request->getParam(PARAM_SafeBoxFIREBASE_SECRET)->value();
      writeFile(SPIFFS, "/SafeBoxFIREBASE_SECRET.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxDelay)) {
      inputMessage = request->getParam(PARAM_SafeBoxDelay)->value();
      writeFile(SPIFFS, "/SafeBoxDelay.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxFTP_Host)) {
      inputMessage = request->getParam(PARAM_SafeBoxFTP_Host)->value();
      writeFile(SPIFFS, "/SafeBoxFTPHost.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxFTP_Username)) {
      inputMessage = request->getParam(PARAM_SafeBoxFTP_Username)->value();
      writeFile(SPIFFS, "/SafeBoxFTPUsername.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxFTP_Password)) {
      inputMessage = request->getParam(PARAM_SafeBoxFTP_Password)->value();
      writeFile(SPIFFS, "/SafeBoxFTPPassword.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxFTP_Port)) {
      inputMessage = request->getParam(PARAM_SafeBoxFTP_Port)->value();
      writeFile(SPIFFS, "/SafeBoxFTPPort.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxAPN)) {
      inputMessage = request->getParam(PARAM_SafeBoxAPN)->value();
      writeFile(SPIFFS, "/SafeBoxAPN.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxUSER)) {
      inputMessage = request->getParam(PARAM_SafeBoxUSER)->value();
      writeFile(SPIFFS, "/SafeBoxUSER.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxPASS)) {
      inputMessage = request->getParam(PARAM_SafeBoxPASS)->value();
      writeFile(SPIFFS, "/SafeBoxPASS.txt", inputMessage.c_str());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();
}

void APgetvalue() {

  Version = readFile(SPIFFS, "/Version.txt");
  Serial.println("Version: " + Version);

  SerialNumber = readFile(SPIFFS, "/SerialNumber.txt");
  Serial.println("SerialNumber: " + SerialNumber);

  SafeBoxName = readFile(SPIFFS, "/SafeBoxName.txt");
  Serial.println("SafeBoxName: " + SafeBoxName);

  SafeBoxPassword = readFile(SPIFFS, "/SafeBoxPassword.txt");
  Serial.println("SafeBoxPassword: " + SafeBoxPassword);

  SafeBoxFIREBASE_URL = readFile(SPIFFS, "/SafeBoxFIREBASE_URL.txt");
  Serial.println("SafeBoxFIREBASE_URL: " + SafeBoxFIREBASE_URL);

  SafeBoxFIREBASE_SECRET = readFile(SPIFFS, "/SafeBoxFIREBASE_SECRET.txt");
  Serial.println("SafeBoxFIREBASE_SECRET: " + SafeBoxFIREBASE_SECRET);

  SafeBoxDelay = readFile(SPIFFS, "/SafeBoxDelay.txt");
  Serial.println("SafeBoxDelay: " + SafeBoxDelay);

  SafeBoxFTPHost = readFile(SPIFFS, "/SafeBoxFTPHost.txt");
  Serial.println("SafeBoxFTPHost: " + SafeBoxFTPHost);

  SafeBoxFTPUsername = readFile(SPIFFS, "/SafeBoxFTPUsername.txt");
  Serial.println("SafeBoxFTPUsername: " + SafeBoxFTPUsername);

  SafeBoxFTPPassword = readFile(SPIFFS, "/SafeBoxFTPPassword.txt");
  Serial.println("SafeBoxFTPPassword: " + SafeBoxFTPPassword);

  SafeBoxFTPPort = readFile(SPIFFS, "/SafeBoxFTPPort.txt");
  Serial.println("SafeBoxFTPPort: " + SafeBoxFTPPort);

  SafeBoxAPN = readFile(SPIFFS, "/SafeBoxAPN.txt");
  Serial.println("SafeBoxAPN: " + SafeBoxAPN);

  SafeBoxUSER = readFile(SPIFFS, "/SafeBoxUSER.txt");
  Serial.println("SafeBoxUSER: " + SafeBoxUSER);

  SafeBoxPASS = readFile(SPIFFS, "/SafeBoxPASS.txt");
  Serial.println("SafeBoxPASS: " + SafeBoxPASS);

}

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

void read_file(String pattern, String file_name) {
  file.DataOutput =  data_out;
  file.ReadFile(pattern, file_name);
}

void detectSwitch() {
  count += 1;
}

void APsetup() {
  EEPROM.write(0, 1);
  EEPROM.commit();
  delay(1000);
  ESP.restart();
  delay(1000);
}

void APrestory() {
  EEPROM.write(0, 0);
  EEPROM.commit();
  delay(1000);
  ESP.restart();
  delay(1000);
}

void setup() {

  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  mac_address_cut = WiFi.macAddress();
  mac_address += getValue(mac_address_cut, ':', 3) + getValue(mac_address_cut, ':', 4) + getValue(mac_address_cut, ':', 5);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  sensors.begin();
  pinMode(AS, INPUT);
  pinMode(AR, INPUT);
  pinMode(water1, INPUT);
  pinMode(water2, INPUT);
  pinMode(motion, INPUT);
  pinMode(car, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);

  setups = EEPROM.read(0);
  Serial.println("Setups : " + String(setups));
  Serial.println("Safe-box MAC Address:  " + mac_address);

  switch (setups) {

    case 0 :

      APgetvalue();

      gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
      gsm.Event_debug = debug;

      gsm.SetPowerKeyPin();
      Serial.println();
      Serial.println(F("Safe-box"));
      gsm.PowerOn();
      while (gsm.WaitReady()) {}

      delay(8000);
      Serial.print(F("GetOperator --> "));
      Serial.println(gsm.GetOperator());
      Serial.print(F("SignalQuality --> "));
      Serial.println(gsm.SignalQuality());
      Serial.println(F("Disconnect net"));

      net.DisConnect();
      Serial.println(F("Set APN and Password"));
      net.Configure(SafeBoxAPN, SafeBoxUSER, SafeBoxPASS);
      Serial.println(F("Connect net"));
      net.Connect();
      Serial.println(F("Show My IP"));
      Serial.println(net.GetIP());
      Serial.println(F("Start HTTP"));

      gps.Start();
      Serial.println(F("GPS Start"));
      delay(500);

      firebase.begin(SafeBoxFIREBASE_URL, SafeBoxFIREBASE_SECRET);

      if (int(firebase.connect()) == 1) {

        Serial.println("Create SafeBoxName");
        firebase.setStr("boxes/" + mac_address + "/boxName", SafeBoxName);

        Serial.println("Create SafeBoxPassword");
        firebase.setStr("boxes/" + mac_address + "/boxPassword", SafeBoxPassword);

        // new
        Serial.println("Create Users");
        firebase.setStr("boxes/" + mac_address + "/users/aaa@aaa", "admin");
        firebase.setStr("boxes/" + mac_address + "/users/bbb@bbb", "viewer");

        Serial.println("Create Config");
        firebase.setInt("boxes/" + mac_address + "/cbmotion", 60);
        firebase.setInt("boxes/" + mac_address + "/cbtemp", 50);
        firebase.setInt("boxes/" + mac_address + "/setinterval", 50);
        firebase.setInt("boxes/" + mac_address + "/soundRequest", 0);

      }
      firebase.close();

      previousMillis1 = millis();
      previousMillis2 = millis();
      previousMillis3 = millis();
      previousMillis4 = millis();
      previousMillis5 = millis();

      attachInterrupt(digitalPinToInterrupt(motion), detectSwitch, FALLING);
      break;

    case 1 :

      Serial.println();
      Serial.print("Setting AP (Access Point)…");
      WiFi.softAP(SafeBoxName.c_str(), SafeBoxPassword.c_str());

      IPAddress IP = WiFi.softAPIP();
      Serial.println();
      Serial.print("AP IP address: ");
      Serial.println(IP);

      APconifg();
      break;
  }
}

void loop() {

  if (digitalRead(AS) == LOW) {
    Serial.println("----------------------------");
    Serial.println("--- AP SETUP ---");
    Serial.println("----------------------------");
    APsetup();
  }

  if (digitalRead(AR) == LOW) {
    Serial.println("----------------------------");
    Serial.println("--- AP Restory ---");
    Serial.println("----------------------------");
    APrestory();
  }

  if (setups == 0) {

    sensors.requestTemperatures();
    temperatureC = sensors.getTempCByIndex(0);

    String GPS_DATA = gps.GetPosition();
    Serial.println(GPS_DATA);
    latitude = getValue(GPS_DATA, ',', 1 );
    longitude = getValue(GPS_DATA, ',', 2 );

    if (latitude == "" && longitude == "") {
      Serial.println("GPS Wait.....");
    } else {
      Serial.println("latitude : " + latitude);
      Serial.println("longitude : " + longitude);
    }

    Serial.println("mac_address : " + mac_address);

    if (int(firebase.connect()) == 1) {
      micrecord = firebase.getInt("boxes/" + mac_address + "/soundRequest");
      cbtemp = firebase.getInt("boxes/" + mac_address + "/cbtemp");
      cbmotion =  firebase.getInt("boxes/" + mac_address + "/cbmotion");
      setinterval = firebase.getInt("boxes/" + mac_address + "/setinterval");
      Serial.println("----------------------------");
      Serial.println("micrecord:" + String(micrecord));
      Serial.println("cbtemp:" + String(cbtemp));
      Serial.println("cbmotion:" + String(cbmotion));
      Serial.println("Setinterval:" + String(setinterval));
      Serial.println("----------------------------");
    }
    firebase.close();

    Serial.println("----------------------------");
    Serial.println("Interrupt:" + String(count));
    Serial.println("----------------------------");
    if (count > cbmotion) {
      motion_sensor = 1;
    }

    if (digitalRead(car) == HIGH) {
      unsigned long currentMillis1 = millis();
      if (currentMillis1 - previousMillis1 >=  60 * setinterval) {
        Serial.println("----------------------------");
        Serial.println("Car stop Initial Mode in!");
        Serial.println("----------------------------");
        runs = true;
        previousMillis1 = currentMillis1;
      }
    }
    if (digitalRead(car) == LOW) {
      Serial.println("----------------------------");
      Serial.println("Car start Initial Mode out!");
      Serial.println("----------------------------");
      runs = false;
    }

    Serial.println("----------------------------");
    Serial.println("SATET RUN : " + String(runs));
    Serial.println("----------------------------");

    if (runs == true && digitalRead(water1) == LOW && digitalRead(water2) == LOW && aroundstate == false) {
      water_sensor_1 = 1;
      aroundstate = true;
      unsigned long currentMillis2 = millis();
      if (currentMillis2 - previousMillis2 >=  60 * setinterval) {
        Serial.println("****************************");
        Serial.println("Car stop case Water1");
        Serial.println("****************************");
        if (int(firebase.connect()) == 1) {
          firebase.setInt("boxes/" + mac_address + "/lastWater1", water_sensor_1);
          delay(100);
          firebase.setStr("boxes/" + mac_address + "/lastLatitude",  latitude);
          firebase.setStr("boxes/" + mac_address + "/lastLongitude", longitude);
          delay(100);
          aroundstate = false;
        }
        firebase.close();
        previousMillis2 = currentMillis2;
      }
    } else {
      water_sensor_1 = 0;
    }

    if (runs == true && temperatureC > cbtemp && aroundstate == false) {
      aroundstate = true;
      unsigned long currentMillis4 = millis();
      if (currentMillis4 - previousMillis4 >=  60 * setinterval) {
        Serial.println("****************************");
        Serial.println("Car stop case temperature");
        Serial.println("****************************");
        if (int(firebase.connect()) == 1) {
          firebase.setInt("boxes/" + mac_address + "/lastVehicle", int(runs));
          delay(100);
          firebase.setFloat("boxes/" + mac_address + "/lastTemperature", temperatureC);
          delay(100);
          aroundstate = false;
        }
        firebase.close();
        previousMillis4 = currentMillis4;
      }
    }

    if (runs == true && motion_sensor >= 1 && aroundstate == false) {
      aroundstate = true;
      unsigned long currentMillis5 = millis();
      if (currentMillis5 - previousMillis5 >=  60 * setinterval) {
        Serial.println("****************************");
        Serial.println("car stop case motion");
        Serial.println("****************************");
        if (int(firebase.connect()) == 1) {
          firebase.setInt("boxes/" + mac_address + "/lastMotion", motion_sensor);
          delay(100);
          aroundstate = false;
        }
        motion_sensor = 0;
        count = 0;
        firebase.close();
        previousMillis5 = currentMillis5;
      }
    }
    delay(SafeBoxDelay.toInt());
  }
}
