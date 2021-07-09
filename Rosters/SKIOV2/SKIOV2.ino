#include "SPI.h"
#include "Ethernet.h"
#include <WebSocketClient.h>
using namespace net;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

WebSocketClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Starting...");

  // start the Ethernet connection:
  while (Ethernet.begin(mac) != 1) {
    Serial.print(".");
  }
  Serial.print("My IP :");
  Serial.println(Ethernet.localIP());

  client.onOpen([](WebSocket & ws) {
    Serial.println(F("Type a message in the following format: <text>"));
    Serial.println(F("----------------------------------------------"));
  });

  client.onMessage(
    [](WebSocket & ws, const WebSocket::DataType, const char *message,
  uint16_t) {
    Serial.println(message);
  });
  client.onClose([](WebSocket &, const WebSocket::CloseCode, const char *,
  uint16_t) {
    Serial.println(F("Disconnected"));
  });

  if (!client.open("192.168.0.107", 4001)) {
    Serial.println(F("Connection failed!"));
    while (true)
      ;
  }
}


void loop() {

  client.listen();

}
