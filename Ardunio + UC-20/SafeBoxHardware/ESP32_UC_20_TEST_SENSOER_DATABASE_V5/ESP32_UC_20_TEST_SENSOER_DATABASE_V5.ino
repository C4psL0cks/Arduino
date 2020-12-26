// ------------------------------------------------------------------ //
#include <EEPROM.h>
int setups = 0;
int mic = 0;
#define EEPROM_SIZE 2
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
#include <HardwareSerial.h>
HardwareSerial mySerial(2);
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "http.h"
#include "gnss.h"
INTERNET net;
UC_FILE file;
HTTP http;
GNSS gps;
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
#include <OneWire.h>
#include <DallasTemperature.h>
#define temps 32
OneWire oneWire(temps);
DallasTemperature sensors(&oneWire);
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
#include "Arduino.h"
#include <FS.h>
#include "Wav.h"
#include "I2S.h"
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
#define RXPIN  27
#define TXPIN  26
// ------------------------------------------------------------------ //
#define RS 21
#define water1 34
#define water2 35
#define motion 25
#define car 33
#define buzzer 17
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
int water_sensor_1 = 0;
int water_sensor_2 = 0;
int motion_sensor = 0;
int car_sensor = 0;
float temperatureC = 0;
String mac_address = "";
String latitude = "-";
String longitude = "-";
int state = 0;
unsigned long currentMillis = 30 * 1000UL; // s
//unsigned long currentMillis = 3 * 60 * 1000UL; // m
unsigned long last_time = 0;
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#define RS 21
#include <HTTPClient.h>

AsyncWebServer server(80);

String Version = "";
String SerialNumber = "";
String SafeBoxName = "";
String SafeBoxPassword = "";
String SafeBoxURL1 = "";
String SafeBoxURL2 = "";
String SafeBoxDelay = "";
String SafeBoxFTPHost = "";
String SafeBoxFTPUsername = "";
String SafeBoxFTPPassword = "";
String SafeBoxFTPPort = "";
String SafeBoxAPN = "";
String SafeBoxUSER = "";
String SafeBoxPASS = "";

const char* ssid     = "Safe-Box";
const char* password = "";

const char* PARAM_Version = "Version";
const char* PARAM_SerialNumber = "SerialNumber";
const char* PARAM_SafeBoxName = "SafeBoxName";
const char* PARAM_SafeBoxPassword = "SafeBoxPassword";

const char* PARAM_SafeBoxURL1 = "SafeBoxURL1";
const char* PARAM_SafeBoxURL2 = "SafeBoxURL2";
const char* PARAM_SafeBoxDelay = "SafeBoxDelay";

const char* PARAM_SafeBoxFTP_Host = "SafeBoxFTPHost";
const char* PARAM_SafeBoxFTP_Username = "SafeBoxFTPUsername";
const char* PARAM_SafeBoxFTP_Password = "SafeBoxFTPPassword";
const char* PARAM_SafeBoxFTP_Port = "SafeBoxFTPPort";

const char* PARAM_SafeBoxAPN = "SafeBoxAPN";
const char* PARAM_SafeBoxUSER = "SafeBoxUSER";
const char* PARAM_SafeBoxPASS = "SafeBoxPASS";

void APconifg() {

  Serial.println();
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.println();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(IP);

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
    else if (request->hasParam(PARAM_SafeBoxURL1)) {
      inputMessage = request->getParam(PARAM_SafeBoxURL1)->value();
      writeFile(SPIFFS, "/SafeBoxURL1.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_SafeBoxURL2)) {
      inputMessage = request->getParam(PARAM_SafeBoxURL2)->value();
      writeFile(SPIFFS, "/SafeBoxURL2.txt", inputMessage.c_str());
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

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Version = readFile(SPIFFS, "/Version.txt");
  Serial.print("*** Your Version: ");
  Serial.println(Version);

  SerialNumber = readFile(SPIFFS, "/SerialNumber.txt");
  Serial.print("*** Your SerialNumber: ");
  Serial.println(SerialNumber);

  SafeBoxName = readFile(SPIFFS, "/SafeBoxName.txt");
  Serial.print("*** Your SafeBoxName: ");
  Serial.println(SafeBoxName);

  SafeBoxPassword = readFile(SPIFFS, "/SafeBoxPassword.txt");
  Serial.print("*** Your SafeBoxPassword: ");
  Serial.println(SafeBoxPassword);

  SafeBoxURL1 = readFile(SPIFFS, "/SafeBoxURL1.txt");
  Serial.print("*** Your SafeBoxURL1: ");
  Serial.println(SafeBoxURL1);

  SafeBoxURL2 = readFile(SPIFFS, "/SafeBoxURL2.txt");
  Serial.print("*** Your SafeBoxURL2: ");
  Serial.println(SafeBoxURL2);

  SafeBoxDelay = readFile(SPIFFS, "/SafeBoxDelay.txt");
  Serial.print("*** Your SafeBoxDelay: ");
  Serial.println(SafeBoxDelay);

  SafeBoxFTPHost = readFile(SPIFFS, "/SafeBoxFTPHost.txt");
  Serial.print("*** Your SafeBoxFTPHost: ");
  Serial.println(SafeBoxFTPHost);

  SafeBoxFTPUsername = readFile(SPIFFS, "/SafeBoxFTPUsername.txt");
  Serial.print("*** Your SafeBoxFTPUsername: ");
  Serial.println(SafeBoxFTPUsername);

  SafeBoxFTPPassword = readFile(SPIFFS, "/SafeBoxFTPPassword.txt");
  Serial.print("*** Your SafeBoxFTPPassword: ");
  Serial.println(SafeBoxFTPPassword);

  SafeBoxFTPPort = readFile(SPIFFS, "/SafeBoxFTPPort.txt");
  Serial.print("*** Your SafeBoxFTPPort: ");
  Serial.println(SafeBoxFTPPort);

  SafeBoxAPN = readFile(SPIFFS, "/SafeBoxAPN.txt");
  Serial.print("*** Your SafeBoxAPN: ");
  Serial.println(SafeBoxAPN);

  SafeBoxUSER = readFile(SPIFFS, "/SafeBoxUSER.txt");
  Serial.print("*** Your SafeBoxUSER: ");
  Serial.println(SafeBoxUSER);

  SafeBoxPASS = readFile(SPIFFS, "/SafeBoxPASS.txt");
  Serial.print("*** Your SafeBoxPASS: ");
  Serial.println(SafeBoxPASS);
}
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //
void SDInit() {

  SD.begin(CS);
  if (!SD.begin(CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  SD.remove(filename);
  files = SD.open(filename, FILE_WRITE);
  if (!files) return;

  byte header[headerSize];
  wavHeader(header, FLASH_RECORD_SIZE);

  files.write(header, headerSize);
  listSD();
}

void i2sInit() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 64,
    .dma_buf_len = 1024,
    .use_apll = 1
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len) {
  uint32_t j = 0;
  uint32_t dac_value = 0;
  for (int i = 0; i < len; i += 2) {
    dac_value = ((((uint16_t) (s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
    d_buff[j++] = 0;
    d_buff[j++] = dac_value * 256 / 2048;
  }
}

void i2s_adc(void *arg) {
  int i2s_read_len = I2S_READ_LEN;
  int flash_wr_size = 0;
  size_t bytes_read;

  char* i2s_read_buff = (char*) calloc(i2s_read_len, sizeof(char));
  uint8_t* flash_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));

  i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
  // i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);

  Serial.println(" *** Recording Start *** ");
  while (flash_wr_size < FLASH_RECORD_SIZE) {
    //read data from I2S bus, in this case, from ADC.
    i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
    // example_disp_buf((uint8_t*) i2s_read_buff, 64);
    //save original data from I2S(ADC) into flash.
    i2s_adc_data_scale(flash_write_buff, (uint8_t*)i2s_read_buff, i2s_read_len);
    files.write((const byte*) flash_write_buff, i2s_read_len);
    flash_wr_size += i2s_read_len;
    ets_printf("Sound recording %u%%\n", flash_wr_size * 100 / FLASH_RECORD_SIZE);
    ets_printf("Never Used Stack Size: %u\n", uxTaskGetStackHighWaterMark(NULL));
  }
  files.close();

  free(i2s_read_buff);
  i2s_read_buff = NULL;
  free(flash_write_buff);
  flash_write_buff = NULL;

  listSD();
  vTaskDelete(NULL);
}

void example_disp_buf(uint8_t* buf, int length) {
  printf("======\n");
  for (int i = 0; i < length; i++) {
    printf("%02x ", buf[i]);
    if ((i + 1) % 8 == 0) {
      printf("\n");
    }
  }
  printf("======\n");
}


void listSD() {
  Serial.println(F("\r\nListing SD files:"));
  static const char line[] PROGMEM =  "=================================================";

  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));

  fs::File root = SD.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.println(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(FPSTR(line));
  Serial.println();
  delay(500);
}
// ------------------------------------------------------------------ //
// ------------------------------------------------------------------ //

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

void APsetup() {
  EEPROM.write(0, 1);
  EEPROM.commit();
  delay(500);
  ESP.restart();
  delay(500);
}

void APrestory() {
  EEPROM.write(0, 0);
  EEPROM.commit();
  delay(500);
  ESP.restart();
  delay(500);
}

void MicsRecord() {
  EEPROM.write(1, 1);
  EEPROM.commit();
  delay(500);
  ESP.restart();
  delay(500);
}

void MicsRestory() {
  EEPROM.write(1, 0);
  EEPROM.commit();
  delay(500);
  ESP.restart();
  delay(500);
}


void setup() {

  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  Serial.println("Safe-box MAC Address:  ");
  mac_address = WiFi.macAddress();
  Serial.println(mac_address);
  delay(100);

  sensors.begin();
  pinMode(RS, INPUT);
  pinMode(water1, INPUT);
  pinMode(water2, INPUT);
  pinMode(motion, INPUT);
  pinMode(car, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  delay(100);

  setups = EEPROM.read(0);
  mic = EEPROM.read(1);

  if (setups == 1) {
    APconifg();
  } else {
    APgetvalue();

    SDInit();
    i2sInit();

    if (mic == 1) {
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

      Serial.println(F("Wait mic"));
      xTaskCreate(i2s_adc, "i2s_adc", 1024 * 2, NULL, 1, NULL);

    } else {

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
    }
  }
}

void loop() {

  //  if (digitalRead(RS) == LOW) {
  //      Serial.println("---APSETUP---");
  //      APsetup();
  //  }

  //  if (digitalRead(RS) == LOW) {
  //    Serial.println("---APrestory---");
  //    APrestory();
  //  }

  //    if (digitalRead(RS) == LOW) {
  //      Serial.println("---MicsRecord---");
  //      MicsRecord();
  //    }

  //  if (digitalRead(RS) == LOW) {
  //    Serial.println("---MicsRecord---");
  //    MicsRestory();
  //  }

  String GPS_DATA = gps.GetPosition();
  Serial.println(GPS_DATA);
  if (latitude == "" && longitude == "") {
    latitude = "-";
    longitude = "-";
    Serial.println("---------------------------------------------");
    Serial.println("latitude : " + latitude);
    Serial.println("longitude : " + longitude);
  } else {
    latitude = getValue(GPS_DATA, ',', 1 );
    longitude = getValue(GPS_DATA, ',', 2 );
    Serial.println("---------------------------------------------");
    Serial.println("latitude : " + latitude);
    Serial.println("longitude : " + longitude);
  }

  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);
  water_sensor_1 = digitalRead(water1);
  water_sensor_2 = digitalRead(water2);
  motion_sensor = digitalRead(motion);
  car_sensor = digitalRead(car);
  digitalWrite(buzzer, LOW);
  Serial.println(String(temperatureC) + "ºC");
  Serial.println("Water1:" + String(water_sensor_1));
  Serial.println("Water2:" + String(water_sensor_2));
  Serial.println("motion:" + String(motion_sensor));
  Serial.println("car:" + String(car_sensor));
  Serial.println("---------------------------------------------");

  if (car_sensor == 0 && millis() - last_time > currentMillis) {
    last_time = millis();
    Serial.println("Initial Mode in!");
    state = 1;
  }
 
  if (car_sensor == 1) {
    state = 0;
    Serial.println("Initial Mode out!");
  }

  if (state == 1) {
    Serial.println("Hello arduinonaxxxxxxxx!");
    Serial.println(F("Start HTTP"));
    http.begin(1);
    Serial.println(F("Send HTTP GET"));
    http.url(SafeBoxURL1+"get?macaddress_uaa=" + mac_address
             + "&latitude=" + latitude
             + "&longitude=" + longitude
             + "&water_sensor_1=" + water_sensor_1
             + "&water_sensor_2=" + water_sensor_2
             + "&motion_sensor=" + motion_sensor
             + "&car_sensor=" + car_sensor
             + "&temperatureC=" + temperatureC);
    Serial.println(http.get());
    Serial.println();
    Serial.println(F("Clear data in RAM"));
    file.Delete(RAM, "*");
    Serial.println(F("Save HTTP Response To RAM"));
    http.SaveResponseToMemory(RAM, "web.hml");
    Serial.println(F("Read data in RAM"));
    read_file(RAM, "web.hml");
    Serial.println(F("Disconnect net"));
    net.DisConnect();
  }
}

void Read() {
  while (gsm.available()) {
    Serial.write(gsm.read());
  }
  while (Serial.available()) {
    gsm.write(Serial.read());
  }
}
