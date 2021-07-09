#include <WebSocketServer.h>
using namespace net;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 107);

constexpr uint16_t port = 4001;
WebSocketServer wss{port};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("Initializing ... "));

  // start the Ethernet connection:
  while (Ethernet.begin(mac) != 1) {
    Serial.print(".");
  }
  Serial.print("My IP :");
  Serial.println(Ethernet.localIP());


  wss.onConnection([](WebSocket & ws) {
    const auto protocol = ws.getProtocol();
    if (protocol) {
      Serial.print(F("Client protocol: "));
      Serial.println(protocol);
    }

    ws.onMessage([](WebSocket & ws, const WebSocket::DataType dataType, const char *message, uint16_t length) {
      switch (dataType) {
        case WebSocket::DataType::TEXT:
          Serial.print(F("Received: "));
          Serial.println(message);
          break;
        case WebSocket::DataType::BINARY:
          Serial.println(F("Received binary data"));
          break;
      }
      ws.send(dataType, message, length);
    });

    ws.onClose([](WebSocket &, const WebSocket::CloseCode, const char *, uint16_t) {
      Serial.println(F("Disconnected"));
    });

    Serial.print(F("New client: "));
    Serial.println(ws.getRemoteIP());

    //    const char message[] {"Hello from Arduino server!"};
    //    ws.send(WebSocket::DataType::TEXT, message, strlen(message));
  });

  wss.begin();
}

void loop() {
  wss.listen();
}
