// ------------------------------------------------------------------ //
#include <EEPROM.h>
#include <HardwareSerial.h>

#include <driver/i2s.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "http.h"
#include "gnss.h"

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// ------------------------------------------------------------------ //
#define EEPROM_SIZE 2

#define RXPIN  27
#define TXPIN  26

#define temps 32
#define RS 21
#define CS 5
#define water1 34
#define water2 35
#define motion 25
#define car 33
#define buzzer 17

// ------------------------------------------------------------------ //
#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define I2S_READ_LEN      (16 * 1024)
#define RECORD_TIME       (10) //Seconds
#define I2S_CHANNEL_NUM   (1)
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)

File files;
const char filename[] = "/recording.wav";
const int headerSize = 44;

// ------------------------------------------------------------------ //
HardwareSerial mySerial(2);
OneWire oneWire(temps);
DallasTemperature sensors(&oneWire);
AsyncWebServer server(80);

INTERNET net;
UC_FILE file;
HTTP http;
GNSS gps;

// ------------------------------------------------------------------ //
int setups = 0;
int mic = 0;
int state = 0;
int water_sensor_1 = 0;
int water_sensor_2 = 0;
int motion_sensor = 0;
int car_sensor = 0;
float temperatureC = 0;
String mac_address = "";
String latitude = "-";
String longitude = "-";
unsigned long currentMillis = 30 * 1000UL; // s // 3 * 60 * 1000UL; // m
unsigned long last_time = 0;
// ------------------------------------------------------------------ //

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
       SafeBox URL 1 <br><input type="text" placeholder="SafeBoxURL1" name="SafeBoxURL1"> <br>(current value %SafeBoxURL1%) <br>
       <input type="submit" value="Submit" onclick="submitMessage()">
   </form><br>
   <form action="/get" target="hidden-form">
       SafeBox URL 2 <br><input type="text" placeholder="SafeBoxURL2" name="SafeBoxURL2"> <br>(current value %SafeBoxURL2%) <br>
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
    Serial.println("- empty file or failed to open file");
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
    Serial.println("- failed to open file for writing");
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
  else if (var == "SafeBoxURL1") {
    return readFile(SPIFFS, "/SafeBoxURL1.txt");
  }
  else if (var == "SafeBoxURL2") {
    return readFile(SPIFFS, "/SafeBoxURL2.txt");
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
  Serial.print("Version: ");
  Serial.println(Version);

  SerialNumber = readFile(SPIFFS, "/SerialNumber.txt");
  Serial.print("SerialNumber: ");
  Serial.println(SerialNumber);

  SafeBoxName = readFile(SPIFFS, "/SafeBoxName.txt");
  Serial.print("SafeBoxName: ");
  Serial.println(SafeBoxName);

  SafeBoxPassword = readFile(SPIFFS, "/SafeBoxPassword.txt");
  Serial.print("SafeBoxPassword: ");
  Serial.println(SafeBoxPassword);

  SafeBoxURL1 = readFile(SPIFFS, "/SafeBoxURL1.txt");
  Serial.print("SafeBoxURL1: ");
  Serial.println(SafeBoxURL1);

  SafeBoxURL2 = readFile(SPIFFS, "/SafeBoxURL2.txt");
  Serial.print("SafeBoxURL2: ");
  Serial.println(SafeBoxURL2);

  SafeBoxDelay = readFile(SPIFFS, "/SafeBoxDelay.txt");
  Serial.print("SafeBoxDelay: ");
  Serial.println(SafeBoxDelay);

  SafeBoxFTPHost = readFile(SPIFFS, "/SafeBoxFTPHost.txt");
  Serial.print("SafeBoxFTPHost: ");
  Serial.println(SafeBoxFTPHost);

  SafeBoxFTPUsername = readFile(SPIFFS, "/SafeBoxFTPUsername.txt");
  Serial.print("SafeBoxFTPUsername: ");
  Serial.println(SafeBoxFTPUsername);

  SafeBoxFTPPassword = readFile(SPIFFS, "/SafeBoxFTPPassword.txt");
  Serial.print("SafeBoxFTPPassword: ");
  Serial.println(SafeBoxFTPPassword);

  SafeBoxFTPPort = readFile(SPIFFS, "/SafeBoxFTPPort.txt");
  Serial.print("SafeBoxFTPPort: ");
  Serial.println(SafeBoxFTPPort);

  SafeBoxAPN = readFile(SPIFFS, "/SafeBoxAPN.txt");
  Serial.print("SafeBoxAPN: ");
  Serial.println(SafeBoxAPN);

  SafeBoxUSER = readFile(SPIFFS, "/SafeBoxUSER.txt");
  Serial.print("SafeBoxUSER: ");
  Serial.println(SafeBoxUSER);

  SafeBoxPASS = readFile(SPIFFS, "/SafeBoxPASS.txt");
  Serial.print("SafeBoxPASS: ");
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

void wavHeader(byte* header, int wavSize) {
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSize = wavSize + headerSize - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;
  header[21] = 0x00;
  header[22] = 0x01;
  header[23] = 0x00;
  header[24] = 0x80;
  header[25] = 0x3E;
  header[26] = 0x00;
  header[27] = 0x00;
  header[28] = 0x00;
  header[29] = 0x7D;
  header[30] = 0x00;
  header[31] = 0x00;
  header[32] = 0x02;
  header[33] = 0x00;
  header[34] = 0x10;
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);
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
  pinMode(RS, INPUT); pinMode(water1, INPUT);
  pinMode(water2, INPUT); pinMode(motion, INPUT);
  pinMode(car, INPUT); pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  delay(100);

  setups = EEPROM.read(0);
  mic = EEPROM.read(1);
  Serial.println(setups);
  Serial.println(mic);
  
  if (setups == 1) {
    APconifg();
  } else {
    APgetvalue();

    SDInit();
    i2sInit();

    if (mic == 1) {

      Serial.println(F("Wait mic"));
      xTaskCreate(i2s_adc, "i2s_adc", 1024 * 2, NULL, 1, NULL);
      delay(500);
      
      gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
      gsm.Event_debug = debug;

      gsm.SetPowerKeyPin();
      Serial.println();
      Serial.println(F("Safe-box"));
      gsm.PowerOn();
      while (gsm.WaitReady()) {}

      delay(10000);
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
    
    } else {

      gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
      gsm.Event_debug = debug;

      gsm.SetPowerKeyPin();
      Serial.println();
      Serial.println(F("Safe-box"));
      gsm.PowerOn();
      while (gsm.WaitReady()) {}

      delay(10000);
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
  
  //  if (digitalRead(RS) == LOW) {
  //    Serial.println("---MicsRecord---");
  //    MicsRecord();
  //  }

  if (digitalRead(RS) == LOW) {
    Serial.println("---MicsRestory---");
    MicsRestory();
  }

  String GPS_DATA = gps.GetPosition();
  Serial.println(GPS_DATA);
  
  if (latitude == "" && longitude == "") {
    latitude = "-";
    longitude = "-";
  } else {
    latitude = getValue(GPS_DATA, ',', 1 );
    longitude = getValue(GPS_DATA, ',', 2 );
  }
  
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);
  water_sensor_1 = digitalRead(water1);
  water_sensor_2 = digitalRead(water2);
  motion_sensor = digitalRead(motion);
  car_sensor = digitalRead(car);
  digitalWrite(buzzer, LOW);
  Serial.println("latitude : " + latitude);
  Serial.println("longitude : " + longitude);
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
    http.url(SafeBoxURL1 + "get?macaddress_uaa=" + mac_address
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
  delay(1000);
}

void Read() {
  while (gsm.available()) {
    Serial.write(gsm.read());
  }
  while (Serial.available()) {
    gsm.write(Serial.read());
  }
}
