#include <HardwareSerial.h>
#include "ESP32_UC20.h"
#include "internet.h"
#include "File.h"
#include "ftp.h"
#include "uc_audio.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define RXPIN  27
#define TXPIN  26
#define AS 21
#define AR 13

//FTP Server
#define USERNAME  "devlogtester"
#define PASSWORD  "h4ck3r2539"
#define HOST      "files.000webhost.com"
#define PORT      21

#define PATH      ""
#define FILENAME  "A.wav"
INTERNET net;
UC_FILE file;
FTP ftp;
UCxAUDIO audio;

HardwareSerial mySerial(2);

unsigned long currentMillis;
unsigned long Millis;
int state = 0;

void debug(String data)
{
  Serial.println(data);
}
void data_out(char data)
{
  Serial.write(data);
}
void setup() {

  Serial.begin(115200);
  pinMode(AS, INPUT);
  pinMode(AR, INPUT);
  gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
  gsm.Event_debug = debug;

  gsm.SetPowerKeyPin();
  Serial.println();
  Serial.println(F("Safe-box"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}

  delay(8000);
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
  delay(500);

}

void loop() {

  while (gsm.available()) {
    Serial.write(gsm.read());
  }
  while (Serial.available()) {
    gsm.write(Serial.read());
  }

  if (digitalRead(AS) == LOW) {
    Serial.println("----------------------------");
    Serial.println("******** AP SETUP **********");
    Serial.println("----------------------------");
    state = 1;
  }
  if (state == 1) {
    Record();
    state = 0;
  }

  if (digitalRead(AR) == LOW) {
    Serial.println("----------------------------");
    Serial.println("******* AP RESTORY *********");
    Serial.println("----------------------------");
    state = 2;
  }
  if (state == 2) {
    deletes();
    state = 0;
  }

}
void deletes() {

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

  file.Delete(UFS, "*");
  Serial.println(F("Clear data in UFS"));

  Serial.println(F("Logout"));
  Serial.println(ftp.Logout());
  Serial.println(F("Disconnect net"));
  net.DisConnect();

}

void Record() {

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

  delay(2000);
  Serial.println(F("Start Record"));
  audio.StartWAVRecords(FILENAME, false);
  delay(10000);
  audio.StopRecord();
  Serial.println(F("Stop Record"));
  delay(1000);

  Serial.println(F("FTP PUT To Server"));
  ftp.put(FILENAME, UFS, FILENAME, 0);

  Serial.println(F("List File in Server"));
  ftp.ListOutput = debug;
  ftp.List(".");

  Serial.println(F("Logout"));
  Serial.println(ftp.Logout());
  Serial.println(F("Disconnect net"));
  net.DisConnect();
}
