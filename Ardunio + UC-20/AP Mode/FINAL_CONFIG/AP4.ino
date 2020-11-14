#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* ssid     = "Safe-Box-Hardware";
const char* password = "123456789";

const char* PARAM_Version = "Version";
const char* PARAM_SerialNumber = "SerialNumber";
const char* PARAM_SafeBoxName = "SafeBoxName";
const char* PARAM_SafeBoxPassword = "SafeBoxPassword";

// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
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
    <iframe style="display:none" name="hidden-form"></iframe>
</body>

</html>
)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path) {
  //  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  //  Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  //  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    //Serial.println("- file written");
  } else {
    //Serial.println("- write failed");
  }
}

String processor(const String& var) {
  //Serial.println(var);
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
  return String();
}

void setup() {
  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
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
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();

  String Version = readFile(SPIFFS, "/Version.txt");
  Serial.print("*** Your Version: ");
  Serial.println(Version);

  String SerialNumber = readFile(SPIFFS, "/SerialNumber.txt");
  Serial.print("*** Your SerialNumber: ");
  Serial.println(SerialNumber);

  String SafeBoxName = readFile(SPIFFS, "/SafeBoxName.txt");
  Serial.print("*** Your SafeBoxName: ");
  Serial.println(SafeBoxName);

  String SafeBoxPassword = readFile(SPIFFS, "/SafeBoxPassword.txt");
  Serial.print("*** Your SafeBoxPassword: ");
  Serial.println(SafeBoxPassword);
}

void loop() {

}
