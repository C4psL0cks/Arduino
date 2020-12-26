#include <Arduino.h>

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
