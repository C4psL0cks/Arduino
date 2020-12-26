#include <HardwareSerial.h>
#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "ftp.h"
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#define CS 5

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"
#define RXPIN  27
#define TXPIN  26

//FTP Server
#define USERNAME  "testersloger"
#define PASSWORD  "h4ck3r2539"
#define HOST      "files.000webhost.com"
#define PORT      21

#define PATH      "/test"
//#define FILENAME  "/recording.wav"

#define FILENAME  "web.html"

INTERNET net;
UC_FILE file;
FTP ftp;

HardwareSerial mySerial(2);

void debug(String data) {
  Serial.println(data);
}
void data_out(char data) {
  Serial.write(data);
}
void setup() {

  Serial.begin(115200);

  SD.begin(CS);
  if (!SD.begin(CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
  gsm.Event_debug = debug;

  gsm.SetPowerKeyPin();
  Serial.println(F("ESP32-UC20"));
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
  net.Configure(APN, USER, PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());
  Serial.println(F("Start HTTP"));

  ftp.begin(1);
  Serial.println(F("Set FTP Username and Password"));
  ftp.SetUsernamePassword(USERNAME, PASSWORD);
  Serial.println(F("Set File Type = Binary"));
  ftp.SetFileType(BINARY);
  Serial.println(F("Set FTP = Passive Mode"));
  ftp.SetTransMode(PASSIVE);
  Serial.println(F("Set Timeout"));
  ftp.SetTimeout(90);
  Serial.print(F("Login Server"));
  Serial.println(ftp.LoginServer(HOST, PORT));
  Serial.println(F("Set Path"));
  Serial.println(ftp.SetPath(PATH));

 Serial.println(F("Wriet Data To RAM"));
 String data = "Test send file FTP From UC20";
 write_file(RAM, FILENAME, data);
//
//  Serial.println(F("FTP PUT To Server"));
//  ftp.put(FILENAME, RAM, FILENAME, 0);
//
//  Serial.println(F("List File in Server"));
//  ftp.ListOutput = debug;
//  ftp.List(".");

  Serial.println(F("Clear data in RAM"));
  file.Delete(RAM,"*");
  
  Serial.println(F("Save List To RAM"));
  int ret=ftp.ListToMemory("*",RAM,"list.txt");
  Serial.println(ret);
  
  Serial.println(F("Read data in RAM"));
  read_file(RAM,"list.txt");
  
  Serial.println(ftp.Logout());
  Serial.println(F("Disconnect net"));
  net.DisConnect();

  Serial.println(F("Logout"));
  Serial.println(ftp.Logout());
  Serial.println(F("Disconnect net"));
  net.DisConnect();
}
void write_file(String pattern, String file_name, String data)
{
  int handle = file.Open(pattern, file_name);
  if (handle != -1)
  {
    if (file.BeginWrite(handle, data.length()))
    {
      file.Print(data);
      file.WaitFinish();
    }
  }
  file.Close(handle);
}
void read_file(String pattern, String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern, file_name);
}

void loop()
{
  if (gsm.available())
  {
    Serial.write(gsm.read());
  }
  if (Serial.available())
  {
    char c = Serial.read();
    gsm.write(c);
  }
}
