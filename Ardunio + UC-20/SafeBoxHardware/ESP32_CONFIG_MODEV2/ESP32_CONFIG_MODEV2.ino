#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

const char* ssid     = "Safe-Box-Hardware";
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
    //Serial.println("- file written");
  } else {
    //Serial.println("- write failed");
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
  ///
  else if (var == "SafeBoxURL1") {
    return readFile(SPIFFS, "/SafeBoxURL1.txt");
  }
  else if (var == "SafeBoxURL2") {
    return readFile(SPIFFS, "/SafeBoxURL2.txt");
  }
  else if (var == "SafeBoxDelay") {
    return readFile(SPIFFS, "/SafeBoxDelay.txt");
  }
  ///
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
  ///
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

void setup() {
  Serial.begin(115200);
  pinMode(RS,INPUT);
  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //  SPIFFS.format();
  
  Serial.println();
  Serial.println();
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
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
    ///
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
    //
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
    ///
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
  
  //
  String SafeBoxURL1 = readFile(SPIFFS, "/SafeBoxURL1.txt");
  Serial.print("*** Your SafeBoxURL1: ");
  Serial.println(SafeBoxURL1);

  String SafeBoxURL2 = readFile(SPIFFS, "/SafeBoxURL2.txt");
  Serial.print("*** Your SafeBoxURL2: ");
  Serial.println(SafeBoxURL2);

  String SafeBoxDelay = readFile(SPIFFS, "/SafeBoxDelay.txt");
  Serial.print("*** Your SafeBoxDelay: ");
  Serial.println(SafeBoxDelay);
  
  //
  String SafeBoxFTPHost = readFile(SPIFFS, "/SafeBoxFTPHost.txt");
  Serial.print("*** Your SafeBoxFTPHost: ");
  Serial.println(SafeBoxFTPHost);

  String SafeBoxFTPUsername = readFile(SPIFFS, "/SafeBoxFTPUsername.txt");
  Serial.print("*** Your SafeBoxFTPUsername: ");
  Serial.println(SafeBoxFTPUsername);

  String SafeBoxFTPPassword = readFile(SPIFFS, "/SafeBoxFTPPassword.txt");
  Serial.print("*** Your SafeBoxFTPPassword: ");
  Serial.println(SafeBoxFTPPassword);

  String SafeBoxFTPPort = readFile(SPIFFS, "/SafeBoxFTPPort.txt");
  Serial.print("*** Your SafeBoxFTPPort: ");
  Serial.println(SafeBoxFTPPort);
  
  //
  String SafeBoxAPN = readFile(SPIFFS, "/SafeBoxAPN.txt");
  Serial.print("*** Your SafeBoxAPN: ");
  Serial.println(SafeBoxAPN);

  String SafeBoxUSER = readFile(SPIFFS, "/SafeBoxUSER.txt");
  Serial.print("*** Your SafeBoxUSER: ");
  Serial.println(SafeBoxUSER);

  String SafeBoxPASS = readFile(SPIFFS, "/SafeBoxPASS.txt");
  Serial.print("*** Your SafeBoxPASS: ");
  Serial.println(SafeBoxPASS);
   
}

void loop() {

}
