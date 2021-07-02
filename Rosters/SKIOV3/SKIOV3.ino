#include "SocketIOClient.h"
#include "Ethernet.h"
#include "SPI.h"

SocketIOClient client;

byte mac[] = { 0xAA, 0x00, 0xBE, 0xEF, 0xFE, 0xEE };
char hostname[] = "192.168.0.104/";
int port = 3000;

extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 10000;
void setup() {

  Serial.begin(115200);
  while (Ethernet.begin(mac) != 1) {
    Serial.print(".");
  }
  Serial.print("My IP :");
  Serial.println(Ethernet.localIP());

  if (!client.connect(hostname, port))
    Serial.println(F("Not connected."));
  if (client.connected())
  {
//    client.send("connection", "message", "Connected !!!!");
    client.send("Command", "id", "roasterID");
  }
  else
  {
    Serial.println("Connection Error");
    while (1);
  }
  delay(1000);
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    //client.heartbeat(0);
    client.send("event", "colorChangeEvent", "firstCrackBeginningEvent");
  }
  if (client.monitor())
  {
    Serial.println(RID);
    //    if (RID == "atime" && Rname == "time")
    //    {
    //      Serial.print("Time is ");
    //      Serial.println(Rcontent);
    //    }
  }
}
