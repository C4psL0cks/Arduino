/*  IN1 MDB1500KVA
 *  IN2 MDB1000KVA
 *  IN3 MDB2000KVA#1 
 *  IN4 MDB2000KVA#2 
 *  IN5 PROTECT#1
 *  IN6 PROTECT#2
 */

#include <HardwareSerial.h>

#define IN1PIN  35
#define IN2PIN  32
#define IN3PIN  33
#define IN4PIN  25
#define IN5PIN  26
#define IN6PIN  27
#define buttonPIN 4
#define ledS1PIN  18
#define ledS2PIN  5
#define ledTXPIN  17
#define ledRXPIN  16
#define simOnPIN  19
#define simRXPIN  14
#define simTXPIN  12

#define Authen  "b44JZQYDrtbkqLlPZeNFs8Rm3ieHtGvz9KSbWuubnG1" 
String phone_number1 = "+66811915236";   //"+66618386615";   // ตั้งค่าเบอร์ที่จะให้โทรออก
//String phone_number2 = "+66915384929";   // ตั้งค่าเบอร์ที่จะให้โทรออก
//String phone_number3 = "+66830086696";   // ตั้งค่าเบอร์ที่จะให้โทรออก
String phone_number4 = "+66992966993";   // ตั้งค่าเบอร์ที่จะให้โทรออก
String phone_number5 = "+66815638217";   // ตั้งค่าเบอร์ที่จะให้โทรออก
String phone_number2 = "+66618386615";     // ตั้งค่าเบอร์ที่จะให้โทรออก
String phone_number3 = "+66890423028";     // ตั้งค่าเบอร์ที่จะให้โทรออก
String phone_number6 = "+66890423028";     // ตั้งค่าเบอร์ที่จะให้โทรออก

#define APN "internet"
#define USER "true"
#define PASS "true"

#define OFF HIGH
#define ON  LOW

HardwareSerial MySerial(1);

unsigned long timeGSMOn = 0;
unsigned char flag_line_error=1; 
unsigned char CLI_ID = 4;

String statusMdb1 = "OFF";
String statusMdb2 = "OFF";
String statusMdb3 = "OFF";
String statusMdb4 = "OFF";
String statusPro1 = "LOW";
String statusPro2 = "LOW";

String LineMessage = "";

String preStatusMdb1 = "OFF";
String preStatusMdb2 = "OFF";
String preStatusMdb3 = "OFF";
String preStatusMdb4 = "OFF";
String preStatusPro1 = "LOW";
String preStatusPro2 = "LOW";

unsigned long previousMillis = 0;

void setup() 
{
  pinMode(IN1PIN, INPUT);
  pinMode(IN2PIN, INPUT);
  pinMode(IN3PIN, INPUT);
  pinMode(IN4PIN, INPUT);
  pinMode(IN5PIN, INPUT);
  pinMode(IN6PIN, INPUT);
  pinMode(buttonPIN, INPUT);
  pinMode(ledS1PIN, OUTPUT);    digitalWrite(ledS1PIN, OFF);
  pinMode(ledS2PIN, OUTPUT);    digitalWrite(ledS2PIN, OFF);
  pinMode(ledTXPIN, OUTPUT);    digitalWrite(ledTXPIN, OFF);
  pinMode(ledRXPIN, OUTPUT);    digitalWrite(ledRXPIN, OFF);
  pinMode(simOnPIN, OUTPUT);    digitalWrite(simOnPIN, LOW);
  Serial.begin(9600);
  MySerial.begin(9600, SERIAL_8N1, simRXPIN, simTXPIN);
  gsmON();
  gsmONCheck();
  delay(10000);
  while(MySerial.read() != 'P')
  {
    delay(50);
    digitalWrite(ledS1PIN, ON);
    delay(50);
    digitalWrite(ledS1PIN, OFF);
  }
  delay(5000);
  digitalWrite(ledS1PIN, ON);
  DisConnect();
  netConfigure(APN,USER,PASS);
  netConnect();
  Linebegin(Authen);
}

void loop() 
{
  if((millis() - previousMillis) > 5000)
  {
    readINPUT();  
    previousMillis = millis();
  }
  
  if(digitalRead(buttonPIN) == LOW)
  {
    digitalWrite(ledTXPIN, ON);
    SendALL();
  }else{
    digitalWrite(ledTXPIN, OFF);
  }

  /*while (MySerial.available() > 0) {
    Serial.write(MySerial.read());
  }
  while (Serial.available() > 0) {
    MySerial.write(Serial.read());
  }*/

  if(preStatusMdb1 != statusMdb1)
  {
    SendALL();
  }
  if(preStatusMdb2 != statusMdb2)
  {
    SendALL();
  }
  if(preStatusMdb3 != statusMdb3)
  {
    SendALL();
  }
  if(preStatusMdb4 != statusMdb4)
  {
    SendALL();
  }
  if(preStatusPro1 != statusPro1)
  {
    SendALL();
  }
  if(preStatusPro2 != statusPro2)
  {
    SendALL();
  }

}

void readINPUT()
{
   if(digitalRead(IN1PIN) == LOW)
  {
    statusMdb1 = "OFF";
  }else
  {
    statusMdb1 = "ON";
  }

  if(digitalRead(IN2PIN) == LOW)
  {
    statusMdb2 = "OFF";
  }else{
    statusMdb2 = "ON";
  }

  if(digitalRead(IN3PIN) == LOW)
  {
    statusMdb3 = "OFF";
  }else{
    statusMdb3 = "ON";
  }

  if(digitalRead(IN4PIN) == LOW)
  {
    statusMdb4 = "OFF";
  }else{
    statusMdb4 = "ON";
  }

  if(digitalRead(IN5PIN) == LOW)
  {
    statusPro1 = "LOW";
  }else{
    statusPro1 = "STABLE";
  }

  if(digitalRead(IN6PIN) == LOW)
  {
    statusPro2 = "LOW";
  }else{
    statusPro2 = "STABLE";
  }
}

void SendALL()
{
  LineMessage = "";
  LineMessage = "\r\n";
  LineMessage += "MDB STATUS LINE NOTIFY\r\n";
  LineMessage += "-------------------\r\n";
  LineMessage += "MDB1_1500KVA:\t";
  LineMessage += statusMdb1;
  LineMessage += "\r\n";
  LineMessage += "VOLT#1:\t";
  LineMessage += statusPro1;
  LineMessage += "\r\n";
  LineMessage += " -------- \r\n";
  LineMessage += "MDB2_1000KVA:\t";
  LineMessage += statusMdb2;
  LineMessage += "\r\n";
  LineMessage += "MDB3_2000KVA#1:\t";
  LineMessage += statusMdb3;
  LineMessage += "\r\n";
  LineMessage += "MDB4_2000KVA#2:\t";
  LineMessage += statusMdb4;
  LineMessage += "\r\n";
  LineMessage += "VOLT#2:\t";
  LineMessage += statusPro2;
  LineMessage += "\r\n";
  LineMessage += " -------- \r\n";
  
  preStatusMdb1 = statusMdb1;
  preStatusMdb2 = statusMdb2;
  preStatusMdb3 = statusMdb3;
  preStatusMdb4 = statusMdb4;
  preStatusPro1 = statusPro1;
  preStatusPro2 = statusPro2;

  sendLine("Alert");
  delay(2000);
  digitalWrite(ledTXPIN, ON);
  sendLine(LineMessage);
  digitalWrite(ledTXPIN, OFF);
  delay(100);
  //sendSMS6();
  digitalWrite(ledTXPIN, ON);
  sendSMS1();
  digitalWrite(ledTXPIN, OFF);
  delay(100);
  digitalWrite(ledTXPIN, ON);
  sendSMS2();
  digitalWrite(ledTXPIN, OFF);
  delay(100);
  digitalWrite(ledTXPIN, ON);
  sendSMS3();
  //sendSMS4();
  //sendSMS5();
  digitalWrite(ledTXPIN, OFF);
}

void gsmON()
{
  digitalWrite(simOnPIN, HIGH);
  delay(2000);
  digitalWrite(simOnPIN, LOW);
  delay(3000);
  timeGSMOn = millis();
}

void gsmONCheck()
{
  timeGSMOn = millis();
  
  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(50);
    digitalWrite(ledS1PIN, ON);
    delay(50);
    digitalWrite(ledS1PIN, OFF);
    if(millis()-timeGSMOn > 15000)
    {
      gsmON();
    }
  }
}


//**********************************************************//
//*********************** Send SMS *************************//
//**********************************************************//
void sendSMS1()
{
  digitalWrite(ledS2PIN, ON);
  MySerial.print("AT+CMGF=1\r");                 // Select SMS message format to text mode                 
  CheckOK();    
  MySerial.print("AT+CSMP=17,167,0,0\r");        // Set SMS Text Mode Parameters                 
  CheckOK(); 
  MySerial.print("AT+CSCS=\"GSM\"\r");           // Select TE Character Set                
  CheckOK(); 
  MySerial.print("AT+CMGS=\"");                  // Send to your phone number          
  MySerial.print(phone_number1);
  MySerial.print("\"\r");

  CheckReady();                                 // Check ">" 
  MySerial.print("MDB1_1500KVA: ");        MySerial.println(statusMdb1);            // SMS body 
  delay(100);
  MySerial.print("VOLT#1: ");           MySerial.println(statusPro1); 
  delay(100); 
  MySerial.print("MDB2_1000KVA: ");        MySerial.println(statusMdb2); 
  delay(100); 
  MySerial.print("MDB3_2000KVA#1: ");        MySerial.println(statusMdb3);
  delay(100);
  MySerial.print("MDB4_2000KVA#2: ");        MySerial.println(statusMdb4); 
  delay(100);
  MySerial.print("VOLT#2: ");           MySerial.println(statusPro2);
  delay(100); 
  MySerial.write(0x1A);                          // Ctrl+Z 

  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(300);
  }

  digitalWrite(ledS2PIN, OFF);
}// End sendSMS

//**********************************************************//
//*********************** Send SMS *************************//
//**********************************************************//
void sendSMS2()
{
  digitalWrite(ledS2PIN, ON);
  MySerial.print("AT+CMGF=1\r");                 // Select SMS message format to text mode                 
  CheckOK();    
  MySerial.print("AT+CSMP=17,167,0,0\r");        // Set SMS Text Mode Parameters                 
  CheckOK(); 
  MySerial.print("AT+CSCS=\"GSM\"\r");           // Select TE Character Set                
  CheckOK(); 
  MySerial.print("AT+CMGS=\"");                  // Send to your phone number          
  MySerial.print(phone_number2);
  MySerial.print("\"\r");

  CheckReady();                                 // Check ">" 
  MySerial.print("MDB1_1500KVA: ");        MySerial.println(statusMdb1);            // SMS body 
  delay(100);
  MySerial.print("VOLT#1: ");           MySerial.println(statusPro1); 
  delay(100); 
  MySerial.print("MDB2_1000KVA: ");        MySerial.println(statusMdb2); 
  delay(100); 
  MySerial.print("MDB3_2000KVA#1: ");        MySerial.println(statusMdb3);
  delay(100);
  MySerial.print("MDB4_2000KVA#2: ");        MySerial.println(statusMdb4); 
  delay(100);
  MySerial.print("VOLT#2: ");           MySerial.println(statusPro2);
  delay(100); 
  MySerial.write(0x1A);                          // Ctrl+Z 

  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(300);
  }

  digitalWrite(ledS2PIN, OFF);
}// End sendSMS

//**********************************************************//
//***************** Check OK From UC15 *********************//
//**********************************************************//

//**********************************************************//
//*********************** Send SMS *************************//
//**********************************************************//
void sendSMS3()
{
  digitalWrite(ledS2PIN, ON);
  MySerial.print("AT+CMGF=1\r");                 // Select SMS message format to text mode                 
  CheckOK();    
  MySerial.print("AT+CSMP=17,167,0,0\r");        // Set SMS Text Mode Parameters                 
  CheckOK(); 
  MySerial.print("AT+CSCS=\"GSM\"\r");           // Select TE Character Set                
  CheckOK(); 
  MySerial.print("AT+CMGS=\"");                  // Send to your phone number          
  MySerial.print(phone_number3);
  MySerial.print("\"\r");

  CheckReady();                                 // Check ">" 
  MySerial.print("MDB1_1500KVA: ");        MySerial.println(statusMdb1);            // SMS body 
  delay(100);
  MySerial.print("VOLT#1: ");           MySerial.println(statusPro1); 
  delay(100); 
  MySerial.print("MDB2_1000KVA: ");        MySerial.println(statusMdb2); 
  delay(100); 
  MySerial.print("MDB3_2000KVA#1: ");        MySerial.println(statusMdb3);
  delay(100);
  MySerial.print("MDB4_2000KVA#2: ");        MySerial.println(statusMdb4); 
  delay(100);
  MySerial.print("VOLT#2: ");           MySerial.println(statusPro2);
  delay(100); 
  MySerial.write(0x1A);                          // Ctrl+Z 

  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(300);
  }

  digitalWrite(ledS2PIN, OFF);
}// End sendSMS

//**********************************************************//
//***************** Check OK From UC15 *********************//
//**********************************************************//

//**********************************************************//
//*********************** Send SMS *************************//
//**********************************************************//
void sendSMS4()
{
  digitalWrite(ledS2PIN, ON);
  MySerial.print("AT+CMGF=1\r");                 // Select SMS message format to text mode                 
  CheckOK();    
  MySerial.print("AT+CSMP=17,167,0,0\r");        // Set SMS Text Mode Parameters                 
  CheckOK(); 
  MySerial.print("AT+CSCS=\"GSM\"\r");           // Select TE Character Set                
  CheckOK(); 
  MySerial.print("AT+CMGS=\"");                  // Send to your phone number          
  MySerial.print(phone_number4);
  MySerial.print("\"\r");

  CheckReady();                                 // Check ">" 
  MySerial.print("MDB1_1500KVA: ");        MySerial.println(statusMdb1);            // SMS body 
  delay(100);
  MySerial.print("VOLT#1: ");           MySerial.println(statusPro1); 
  delay(100); 
  MySerial.print("MDB2_1000KVA: ");        MySerial.println(statusMdb2); 
  delay(100); 
  MySerial.print("MDB3_2000KVA#1: ");        MySerial.println(statusMdb3);
  delay(100);
  MySerial.print("MDB4_2000KVA#2: ");        MySerial.println(statusMdb4); 
  delay(100);
  MySerial.print("VOLT#2: ");           MySerial.println(statusPro2);
  delay(100); 
  MySerial.write(0x1A);                          // Ctrl+Z 

  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(300);
  }

  digitalWrite(ledS2PIN, OFF);
}// End sendSMS

//**********************************************************//
//***************** Check OK From UC15 *********************//
//**********************************************************//

//**********************************************************//
//*********************** Send SMS *************************//
//**********************************************************//
void sendSMS5()
{
  digitalWrite(ledS2PIN, ON);
  MySerial.print("AT+CMGF=1\r");                 // Select SMS message format to text mode                 
  CheckOK();    
  MySerial.print("AT+CSMP=17,167,0,0\r");        // Set SMS Text Mode Parameters                 
  CheckOK(); 
  MySerial.print("AT+CSCS=\"GSM\"\r");           // Select TE Character Set                
  CheckOK(); 
  MySerial.print("AT+CMGS=\"");                  // Send to your phone number          
  MySerial.print(phone_number5);
  MySerial.print("\"\r");

  CheckReady();                                 // Check ">" 
  MySerial.print("MDB1_1500KVA: ");        MySerial.println(statusMdb1);            // SMS body 
  delay(100);
  MySerial.print("VOLT#1: ");           MySerial.println(statusPro1); 
  delay(100); 
  MySerial.print("MDB2_1000KVA: ");        MySerial.println(statusMdb2); 
  delay(100); 
  MySerial.print("MDB3_2000KVA#1: ");        MySerial.println(statusMdb3);
  delay(100);
  MySerial.print("MDB4_2000KVA#2: ");        MySerial.println(statusMdb4); 
  delay(100);
  MySerial.print("VOLT#2: ");           MySerial.println(statusPro2);
  delay(100); 
  MySerial.write(0x1A);                          // Ctrl+Z 

  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(300);
  }

  digitalWrite(ledS2PIN, OFF);
}// End sendSMS

//**********************************************************//
//*********************** Send SMS *************************//
//**********************************************************//
void sendSMS6()
{
  digitalWrite(ledS2PIN, ON);
  MySerial.print("AT+CMGF=1\r");                 // Select SMS message format to text mode                 
  CheckOK();    
  MySerial.print("AT+CSMP=17,167,0,0\r");        // Set SMS Text Mode Parameters                 
  CheckOK(); 
  MySerial.print("AT+CSCS=\"GSM\"\r");           // Select TE Character Set                
  CheckOK(); 
  MySerial.print("AT+CMGS=\"");                  // Send to your phone number          
  MySerial.print(phone_number6);
  MySerial.print("\"\r");

  CheckReady();                                 // Check ">" 
  MySerial.print("MDB1_1500KVA: ");        MySerial.println(statusMdb1);            // SMS body 
  delay(100);
  MySerial.print("VOLT#1: ");           MySerial.println(statusPro1); 
  delay(100); 
  MySerial.print("MDB2_1000KVA: ");        MySerial.println(statusMdb2); 
  delay(100); 
  MySerial.print("MDB3_2000KVA#1: ");        MySerial.println(statusMdb3);
  delay(100);
  MySerial.print("MDB4_2000KVA#2: ");        MySerial.println(statusMdb4); 
  delay(100);
  MySerial.print("VOLT#2: ");           MySerial.println(statusPro2);
  delay(100); 
  MySerial.write(0x1A);                          // Ctrl+Z 

  while(MySerial.read()!='K')
  {
    MySerial.print("AT\r");
    delay(300);
  }

  digitalWrite(ledS2PIN, OFF);
}// End sendSMS

//**********************************************************//
//***************** Check OK From UC15 *********************//
//**********************************************************//
void CheckOK()
{
  while(MySerial.read()!='K');               // Check "OK" 
  delay(100);  
  digitalWrite(ledS2PIN, ON);
  delay(80);
  digitalWrite(ledS2PIN, OFF);
  delay(80);
}// End Check OK

//**********************************************************//
//************* Check Ready send SMS From UC15 *************//
//**********************************************************//
void CheckReady()
{
  while(MySerial.read()!='>');          // Check ">" 
  delay(100);  
  digitalWrite(ledS2PIN, ON);
  delay(80);
  digitalWrite(ledS2PIN, OFF);
  delay(80);
}// End Check ">" 


bool DisConnect()
{
  MySerial.println("AT+QIDEACT=1");
  return(wait_ok(10000));
}

bool wait_ok(long time)
{
  return wait_ok_(time,true);
}

bool wait_ok_(long time,bool ack)
{
  unsigned long previousMillis = millis(); 
  while(1)
  {
    String req = MySerial.readStringUntil('\n');
    if(req.indexOf(F("OK")) != -1)
    {
      if(ack)
      Serial.println("OK");
      return(1);
    }
    if(req.indexOf(F("ERROR")) != -1)
    {
      if(ack)
      Serial.println("Error");
      return(0);
    }
    //debug(req); 
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= time) 
    {
      previousMillis = currentMillis;
      if(ack)
      Serial.println("Error");
      return(0);
    }     
  }
}

bool netConfigure(String apn,String user,String password)
{
  String  str = "AT+QICSGP=1,1,";
      str += "\""+apn+"\",";
      str += "\""+user+"\",";
      str += "\""+password+"\",";
      str += "1";
  MySerial.println(str);
  return(wait_ok(10000));
}

bool netConnect()
{
  MySerial.println(F("AT+QIACT=1"));
  return(wait_ok(10000));
}

bool Linebegin(String auth)
{
  int ret=0;
  unsigned char CONTEX = 1;
  //Authen = auth;
  ret = sslversion(CONTEX,1);
  if(ret==1)
  {
    ret = ciphersuite(CONTEX,"0xFFFF");
    if(ret==1)
    {
      ret = seclevel(CONTEX,0);
      if(ret==1)
      {
        return(true);
      }
      else
      {
        Serial.println("seclevel=  = Error");
        return(false);
      }
    }
    else
    {
      Serial.println("ciphersuite=  = Error");
      return(false);
    }
      
  }
  else
  {
    Serial.println("sslver = Error");
    return(false);
  } 
}

bool sslversion(unsigned char contexid,unsigned char sslver)
{
  //AT+QSSLCFG="sslversion",1,1
  MySerial.print("AT+QSSLCFG=\"sslversion\",");
  MySerial.println(String(contexid)+","+String(sslver));
  return(wait_ok_ndb(3000));
}

bool ciphersuite(unsigned char contexid,String tls_rsa)
{
  MySerial.print("AT+QSSLCFG=\"ciphersuite\",");
  MySerial.println(String(contexid)+","+tls_rsa);
  return(wait_ok_ndb(3000));
}

bool seclevel(unsigned char contexid,unsigned char level)
{
  MySerial.print("AT+QSSLCFG=\"seclevel\",");
  MySerial.println(String(contexid)+","+String(level));
  return(wait_ok_ndb(3000));
}

bool wait_ok_ndb(long time)
{
  return wait_ok_(time,false);
}

void sendLine(String msg)
{
  digitalWrite(ledRXPIN, ON);
  int ret = Lineconnect();
  if(ret==1)
  {   digitalWrite(ledRXPIN, ON);
      Serial.println("Connected");
      LinesendMessage(msg);
      digitalWrite(ledRXPIN, OFF);
  }
  else
  {
    Serial.println("Not Connect");
    digitalWrite(ledRXPIN, ON);
  }
  Lineclose();
}

int Lineconnect()
{
  unsigned char CONTEX = 1;
  return sslopen(1,CONTEX,CLI_ID,"notify-api.line.me","443",0);
}

int sslopen(unsigned char pdpid,unsigned char contexid,unsigned char clientid,String serverid,String port,unsigned char acc_mode)
{
  //AT+QSSLOPEN=1,1,4,"test-491ef.firebaseio.com",443,2
  MySerial.print("AT+QSSLOPEN=");
  MySerial.println(String(pdpid)+","+String(contexid)+","+String(clientid)+",\""+serverid+"\","+port+","+String(acc_mode));

  if(wait_ok_ndb(10000))
  {
    const long interval = 5000; 
    unsigned long previousMillis = millis(); 
    unsigned char flag=1;
    
    while(flag)
    {
      if(MySerial.available())
      {
        String req = MySerial.readStringUntil('\n');
        if(req.indexOf(F("+QSSLOPEN"))!= -1)
        {
          //Serial.println(req);
          int index = req.indexOf(F(","));
          int res = req.substring(index+1).toInt();
          if(res == 0)
            return(1);
          else
            return(res);
        }
      }
      unsigned long currentMillis = millis();
      if(currentMillis - previousMillis >= interval) 
      {
        Serial.println("out");
        previousMillis = currentMillis;
        return(-1);
      }     
    }
  }
  Serial.print("open timeout");
  return(-2);
}

bool LinesendMessage(String message)
{
  if(sslstartSend(CLI_ID,0))
  {
    flag_line_error=0;
    send_http("POST",message);
    sslstopSend();
    sslwaitRead(3000);
    int len = sslread(CLI_ID);
    while(len)
    {
      if(MySerial.available())
      {
        String req = MySerial.readStringUntil('\n');
        //Serial.println(req);
        if(req.indexOf(F("HTTP"))!= -1)
        {
          Serial.println(req);
        }
        if(req.indexOf(F("OK"))!= -1)
        {
          len =0 ;
        }
      }
    }
    
  }
  else
  {
    flag_line_error=1;
    Serial.println("OpenSend = Error");
    return(false);
  }
}

bool sslstartSend(unsigned char contexid,int len)
{
  MySerial.print("AT+QSSLSEND=");
  MySerial.print(String(contexid));
  if(len>0)
  {
    MySerial.print(",");
    MySerial.print(String(len));
  }
  MySerial.println("");
  
  const long interval = 5000; 
  unsigned long previousMillis = millis(); 
  unsigned char flag=1;
  while(flag)
  {
    if(MySerial.available())
    {
      char c = MySerial.read(); 
      //Serial.write(c);
      if(c =='>')
      {
        //Serial.println("\r\n startsend ok");
        return(true);
      }    
    }
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) 
    {
      MySerial.write(0x1A);
      delay(1000);
      Serial.println("Start send out");
      previousMillis = currentMillis;
      return(false);
    }     
  }
}

void  send_http(String http_type,String data)
{
  String msg = "message="+data;
  String out = http_type+" /api/notify HTTP/1.1";
  MySerial.println(out);
  MySerial.println("Host: notify-api.line.me");
  MySerial.print("Authorization: Bearer ");
  MySerial.println(Authen);
  MySerial.println("Content-Type: application/x-www-form-urlencoded");
  MySerial.println("Cache-Control: no-cache");
  MySerial.print("content-length: ");
  MySerial.println(String(msg.length()));
  MySerial.println("Connection: keep-alive");
  MySerial.println("");
  MySerial.println(msg);
  /*
  //out +=  F("Authorization: Bearer ")+String(Authen)+"\r\n");
  //out +=  F("Content-Type: application/x-www-form-urlencoded\r\n");
  //out +=  F("Cache-Control: no-cache\r\n");
  out +=  F("content-length: ")+String(msg.length());
  out +=  F("Connection: keep-alive\r\n");
  out += "\r\n";
  out += msg;
  */
}

bool sslstopSend()
{
  MySerial.write(0x1A);
  const long interval = 10000; 
  unsigned long previousMillis = millis(); 
  unsigned char flag=1;
  while(flag)
  {
    if(MySerial.available())
    {
      String req = MySerial.readStringUntil('\n');
      //Serial.println(req);
      if(req.indexOf(F("SEND OK"))!= -1)
      {
          return(true);
      }
      if(req.indexOf(F("SEND FAIL"))!= -1)
      {
          return(false);
      }
    }
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) 
    {
      MySerial.write(0x1A);
      delay(1000);
      Serial.println("out");
      previousMillis = currentMillis;
      return(false);
    }     
  }
}

bool sslwaitRead(long time)
{
  const long interval = time; 
  unsigned long previousMillis = millis(); 
  unsigned char flag=1;
  while(flag)
  {
    if(MySerial.available())
    {
      String req = MySerial.readStringUntil('\n');
      //Serial.println(req);
      if(req.indexOf(F("+QSSLURC: \"recv\""))!= -1)
      {
        return(true);
        
      }
    
    }
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) 
    {
      Serial.println("Read Timeout");
      previousMillis = currentMillis;
      return(false);
    }     
  }
  
}

int sslread(unsigned char contexid)
{
  MySerial.print(F("AT+QSSLRECV="));
  MySerial.print(String(contexid));
  MySerial.println(",1500");
  
  const long interval = 3000; 
  unsigned long previousMillis = millis(); 
  unsigned char flag=1;
  while(flag)
  {
    if(MySerial.available())
    {
      String req = MySerial.readStringUntil('\n');
      //Serial.println(req);
      
      if(req.indexOf(F("+QSSLRECV:"))!= -1)
      { 
        int index = req.indexOf(":");
        int len = req.substring(index+2).toInt();
        //Serial.println("ssl read len");
        //Serial.println(len);
        return (len);
      }
    
    }
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) 
    {
      Serial.println("Read Timeout");
      previousMillis = currentMillis;
      return(false);
    }     
  }
  
}

bool Lineclose()
{
  return sslclose(CLI_ID);
}

bool sslclose(unsigned char contexid)
{
  MySerial.print("AT+QSSLCLOSE=");
  MySerial.println(String(contexid));
  
  const long interval = 10000; 
  unsigned long previousMillis = millis(); 
  unsigned char flag=1;
  while(flag)
  {
    if(MySerial.available())
    {
      String req = MySerial.readStringUntil('\n');
      //Serial.println(req);
      if(req.indexOf(F("OK"))!= -1)
      {
        String req = MySerial.readStringUntil('\n');
        return(true);
      }
      
      
    }
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) 
    {
      sslstopSend();
      Serial.println("close time out");
      previousMillis = currentMillis;
      return(false);
    }     
  }
  //return(gsm.wait_ok_ndb(15000));
}
