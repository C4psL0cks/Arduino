#include <Ethernet.h>
#include <ArduinoModbus.h>

bool _1s;
unsigned long TimeAct, TimePrev, HoldingResult, InputResult, HeartBeat, i, StartingAddr;
long starts_on, starts_off, starts_gass, damper, air, gass;


EthernetServer EthServer(502);
ModbusTCPServer modbusTCPServer;

void setup() {
  // Ethernet Settings
  byte mac[] = { 0x4E, 0xA0, 0xBE, 0x3F, 0xFE, 0x0F };  // Define MAc address
  byte ip[] = { 192, 168, 0, 108 };                      // Define IP address
  byte subnet[] = { 255, 255, 255, 0 };                 // Define SubNEt mask

  // initialize the ethernet device
  Ethernet.begin(mac, ip, subnet);                      // Assign MAC, IP, and subnet mask
  Serial.begin(9600);
  EthServer.begin();          // start listening for clients
  modbusTCPServer.begin();    // start listening for clients

  // Define Holding register:
  HoldingResult = modbusTCPServer.configureHoldingRegisters(0, 100);
  InputResult = modbusTCPServer.configureInputRegisters(0, 100);

  Serial.print("Holding Reg init result =");
  Serial.print(HoldingResult);
  Serial.print("\n");

  Serial.print("Input Reg init result =");
  Serial.print(InputResult);
  Serial.print("\n");

  Serial.print("Modbus server address=");
  Serial.println(Ethernet.localIP());
  Serial.print("\n");
}

void loop() {
  // Modbus server accept incoming connections
  EthernetClient client = EthServer.available();

  if (client.connected()) {
    modbusTCPServer.accept(client);
    // poll for Modbus TCP requests, while client connected
    modbusTCPServer.poll();
    // Serial.print("poll");
  }

  //---------------------------------------------------------------------------------
  // TIME  clock 1s
  TimeAct = millis();     // Millis value between  0 and  655535

  _1s = false;
  if (  ( (TimeAct > TimePrev) and (TimeAct - TimePrev) > 1000) or ((TimeAct < TimePrev) and (65535 - TimePrev + TimeAct) > 1000 )  ) {
    _1s = true;
    TimePrev = TimeAct;
  }
  //---------------------------------------------------------------------------------
  //  // HeartBeat
  //  if (_1s) {
  //    HeartBeat++;
  //    if (HeartBeat > 255) {
  //      HeartBeat = 0;
  //    }
  //    Serial.print("HeartBeat=");
  //    Serial.println(HeartBeat);
  //    Serial.print("\n");
  //  }
  //---------------------------------------------------------------------------------
  // Modbus server  :

  // holding register 40001: heartbeat (FC3)
  modbusTCPServer.holdingRegisterWrite(0x00, random(30, 200));
  modbusTCPServer.holdingRegisterWrite(0x01, random(30, 200));

  //Buttons and sliders can send out MODBUS Commands via functions 5, 6, 15, 16 and 22. The following commands in the action description are supported.

  // holding register 40500: Command Word (FC6)
  starts_on = modbusTCPServer.holdingRegisterRead(0x02);
  starts_gass = modbusTCPServer.holdingRegisterRead(0x04);

  damper = modbusTCPServer.holdingRegisterRead(0x05);
  air = modbusTCPServer.holdingRegisterRead(0x06);
  gass = modbusTCPServer.holdingRegisterRead(0x07);


  if (_1s) {
    Serial.println("starts_on : " + String(starts_on)); 
    Serial.println("starts_gass : " + String(starts_gass)); 
    Serial.println("damper : " + String(damper)); 
    Serial.println("air    : " + String(air)); 
    Serial.println("gass   : " + String(gass)); 
  }
}
