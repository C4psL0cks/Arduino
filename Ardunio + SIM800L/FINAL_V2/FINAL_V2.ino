#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Config (Use APN corresponding to your service providers configs)
static String apn = "internet";
static String loggingPassword = "qwerty";
static String serverIP = "34.87.122.165";

// Pins where GPS and SIM modules are connected
static const int SimRXPin = 2, SimTXPin = 3;
static const int GPSRXPin = 8, GPSTXPin = 9;
static const int ErrorPin = 10, SimConnectionPin = 12;

// Used baud rates (define based on used modules)
static const uint32_t SimBaudrate = 9600;
static const uint32_t GPSBaud = 9600;
static const uint32_t SerialBaudrate = 9600;

// How frequently we want to send the location (milliseconds)
static const unsigned long frequency = 5000;

// Maximum time to wait SIM module for response
static long maxResponseTime = 30000;

String responseString;
TinyGPSPlus gps;
unsigned long previous = 0;
unsigned long beltTime;
SoftwareSerial sim_ss(SimRXPin, SimTXPin);
SoftwareSerial gps_ss(GPSRXPin, GPSTXPin);

void setup() {

  // Initialize status pins
  pinMode(ErrorPin, OUTPUT);
  pinMode(SimConnectionPin, OUTPUT);
  digitalWrite(ErrorPin, LOW);
  digitalWrite(SimConnectionPin, LOW);

  /*
     Start serial communications. We can only listen to one ss at a time so changing that
     between sim and gps as needed
  */
  Serial.begin(SerialBaudrate);
  sim_ss.begin(SimBaudrate);
  gps_ss.begin(GPSBaud);
  sim_ss.listen();

  Serial.println("Waiting for init");
  // Wait few seconds so that module is able to take AT commands
  delay(5000);
  Serial.println("Init... waiting until module has connected to network");

  // Start AT communication. This sets auto baud and enables module to send data
  sim_ss.println("AT");
  delay(3000);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+CSQ");
  delay(1000);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+CCID");
  delay(1000);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+CREG?");
  delay(1000);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  // Connect and get IP
  sim_ss.println("AT+SAPBR=3,1,Contype,GPRS");
  delay(100);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  // Some more credentials
  sim_ss.write("AT+SAPBR=3,1,\"APN\",\"");
  sim_ss.print(apn);
  sim_ss.write("\"\r\n");
  delay(100);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+SAPBR=3,1,\"USER\",\"true\"");
  delay(100);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+SAPBR=3,1,\"PWD\",\"true\"");
  delay(100);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+SAPBR=1,1");
  delay(1000);
  waitUntilResponse("ERROR");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+SAPBR=2,1");
  delay(2000);
  waitUntilResponse("OK");
  blinkLed(SimConnectionPin);

  sim_ss.println("AT+HTTPINIT");
  delay(100);
  waitUntilResponse("ERROR");
  digitalWrite(SimConnectionPin, HIGH);

  sim_ss.println("AT+HTTPPARA=CID,1");
  delay(100);
  waitUntilResponse("OK");
  digitalWrite(SimConnectionPin, HIGH);

  gps_ss.listen();
  previous = millis();
  Serial.println("starting loop!");
}

void blinkLed(int led) {
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
}

/*
    Read from SIM serial until we get known response. TODO error handling!
 * */
void waitUntilResponse(String response) {
  beltTime = millis();
  responseString = "";
  String totalResponse = "";
  while (responseString.indexOf(response) < 0 && millis() - beltTime < maxResponseTime) {
    readResponse();
    totalResponse = totalResponse + responseString;
    Serial.println(responseString);
  }

  //  if (totalResponse.length() <= 0) {
  //    Serial.println("No response from the module. Check wiring, SIM-card and power!");
  //    digitalWrite(ErrorPin, HIGH);
  //    delay(30000);
  //    exit(0); // No way to recover
  //  }
  //  else if (responseString.indexOf(response) < 0) {
  //    Serial.println("Unexpected response from the module");
  //    Serial.println(totalResponse);
  //    digitalWrite(ErrorPin, HIGH);
  //    delay(30000);
  //    exit(0); // No way to recover
  //  }
}

/*
   Read from serial until we get response line ending with line separator
 * */
void readResponse()
{
  responseString = "";
  while (responseString.length() <= 0 || !responseString.endsWith("\n"))
  {
    tryToRead();

    if (millis() - beltTime > maxResponseTime)
    {
      return;
    }
  }
}

/*
   If we have anything available on the serial, append it to response string
 * */
void tryToRead()
{
  while (sim_ss.available())
  {
    char c = sim_ss.read();  //gets one byte from serial buffer
    responseString += c; //makes the string readString
  }
}

void loop()
{
  // If we have data, decode and log the data
  while (gps_ss.available())
    if (gps.encode(gps_ss.read())) {
      logInfo();
    }
  // Test that we have had something from GPS module within first 10 seconds
  if (millis() - previous > 10000 && gps.charsProcessed() < 10)
  {
    Serial.println("GPS wiring error!");
    while (true);
  }
}

void logInfo() {

  //  Causes us to wait until we have satelite fix
  if (!gps.location.isValid()) {
    Serial.println("Not a valid location. Waiting for satelite data.");
    blinkLed(ErrorPin);
    return;
  }

  //  Only log once per frequency
  if (millis() - previous > frequency) {
    if (gps.location.isUpdated()) {
      sim_ss.listen();
      Serial.println("==========================================");
      Serial.println("Satellite Count:" + String(gps.satellites.value()));
      Serial.println("Latitude:" + String(gps.location.lat(), 6));
      Serial.println("Longitude:" + String(gps.location.lng(), 6));
      Serial.println("Speed MPH:" + String(gps.speed.mph()));
      Serial.println("Altitude Feet:" + String(gps.altitude.feet()));
      Serial.println("==========================================");
      previous = millis();
      String url = "AT+HTTPPARA=\"URL\",\"http://";
      url += serverIP;
      url += "/sims/web/call_apis.php?true=1";
      url += "&latitude=";
      url += String(gps.location.lat(), 6);
      url += "&longitude=";
      url += String(gps.location.lng(), 6);
      url += "\"";
      sim_ss.println(url);
      //      waitUntilResponse("OK");
      digitalWrite(SimConnectionPin, LOW);
      sim_ss.println("AT+HTTPACTION=0");
      //      waitUntilResponse("+HTTPACTION:");
      digitalWrite(SimConnectionPin, HIGH);
      sim_ss.println("AT+HTTPREAD");
      delay(1000);
      //      waitUntilResponse("+HTTPREAD:");
      sim_ss.println("AT+HTTPTERM");
      delay(1000);
      //      waitUntilResponse("OK");
      gps_ss.listen();
    }
  }
}
