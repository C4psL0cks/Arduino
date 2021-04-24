#include <SPI.h>
#include <WiFi101.h>
#include <WebSocketServer.h>  //use NINA-Websocket-master
#include <Base64.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11    // DHT 11

char ssid[] = "XXXX"; //  your network SSID (name)
char pass[] = "XXXX";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

int webSocketPort = 80;
WiFiServer server(webSocketPort);
WebSocketServer webSocketServer;
DHT dht(DHTPIN, DHTTYPE);

String sendSensorData() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return "";
  }
  String data = String(t) + "," + String(h);
  //Serial.println(data);
  return data;
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  delay(100);                       // wait for a second
}
void WiFiConnect() {

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // Attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    //Serial.print(" .");
    // wait 5 seconds to reconnect
    delay(5000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
  Serial.print("WebsoketServer on: http://" ); Serial.print(ip); Serial.println( "/:" + String(webSocketPort) );
}

void setup() {

  //Initialize serial
  Serial.begin(9600);

  // Setup builtin led
  pinMode(LED_BUILTIN, OUTPUT);
  // turn the LED OFF (LOW on disconnect)
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  //Connect WiFI
  WiFiConnect();



  // turn the LED on (HIGH on connect)
  digitalWrite(LED_BUILTIN, HIGH);

  // start the server:
  server.begin();
  //start Temp/Hum sensor
  dht.begin();
}

void loop() {
  String data;
  //Is a client connected?
  WiFiClient client = server.available();

  if (client.connected() && webSocketServer.handshake(client)) { //If client connected and handshake with websocket is true continue

    while (client.connected()) { //if client is connected start to blink and send the data
      blink();

      // This is only to receive the open and close message from the client
      data = webSocketServer.getData();

      if (data.length() > 0) {
        //handleClientData(data);
        Serial.println("Received from Client: " + data);
      }

      //Get data from DHT11 and send to the client
      if ((data = sendSensorData()) != "") {
        webSocketServer.sendData(data);
        //Serial.println(data);
      }
    }
  }

  // wait to let the client disconnect
  delay(100);
}
