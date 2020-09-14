#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "internet.h"
#include "uc_mqtt.h"
#include "DHT.h"

INTERNET net;
UCxMQTT mqtt;

//SIM TRUE  internet
#define APN "internet"      //กำหนด APN ตาม SIM ที่ใช้
#define USER ""       //กำหนดUSER ตาม SIM ที่ใช้
#define PASS ""       //กำหนดPASSWOR ตาม SIM ที่ใช้
//กำหนด server,port,user,password  ที่ได้จากหน้า Overview ของ CloudMQTT
#define MQTT_SERVER      "yourserver.com"
#define MQTT_PORT        "1883"
#define MQTT_ID              "3G"
#define MQTT_USER        "youruser"
#define MQTT_PASSWORD    " yourpassword"

#define SUBSCRIBE "led"
#define LEDPIN 10
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousmqtt = 0;
const long intervalmqtt = 2000;

AltSoftSerial mySerial;

void debug(String data)
{
  Serial.println(data);
}
void setup()
{
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  Serial.begin(9600);
  dht.begin();        //เริ่มใช้งาน DHT
  gsm.begin(&mySerial, 9600);   //กำหนดใช้งาน 3G module ที่ Baud rate 9600
  gsm.Event_debug = debug;    //กำหนดให้ส่งค่า Print Debug ไปยัง fundtion debug
  Serial.println(F("UC20"));
  gsm.PowerOn();      //สั่งให้ Module เริ่มทำงานใหม่
  while (gsm.WaitReady()) {}  //รอจนกว่า Module 3G พร้อมทำงาน
  Serial.print(F("GetOperator --> "));  //แสดง Operator ที่ใช้งาน
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));  //แสดงความแรงของสัญญาณ
  Serial.println(gsm.SignalQuality());

  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS); //ตั้งค่า APN ,USER,PASSWORD
  Serial.println(F("Connect net"));
  net.Connect();          //เชื่อมต่อ Internet
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());      //แสดงหมายเลข IP ที่ได้รับ
  mqtt.callback = callback;     //กำหนด Function ที่จะทำงานเมื่อมี Message ที่ Sub ไว้เข้ามา
  connect_server();       //เรียก Function เพื่อเชื่อมต่อ Server
}
/*
  Callback คือ function ที่เรา map เอาไว้เมื่อมีการ publish topic ที่เรา subscribe เอาไว้ จะมีการเรียก Function นี้ขึ้นมา โดยจะมีการส่ง ชื่อ topic , payload และ length ของ payload มาให้
*/
void callback(String topic , char *payload, unsigned char length)
{
  Serial.println();
  Serial.println(F("%%%%%%%%%%%%%%%%%%%%%%%%%%%%"));
  Serial.print(F("Topic --> "));
  Serial.println(topic);    //แสดง Topic ที่ได้รับ
  payload[length] = 0;  // กำหนดให้ byte สุดท้ายของ payload เป็น 0 (ปิด String)
  String str_data(payload); //เก็บ payload ให้อยู่ในตัวแปล String
  Serial.print(F("Payload --> "));
  Serial.println(str_data); // แสดง payload

  if (topic == "led") // ตรวจสอบว่า Topic คือ led หรือไม่
  {
    if (str_data.indexOf("on") != -1) // ถ้า payload คือคำว่า on ให้ LED ติด
      digitalWrite(LEDPIN, LOW);
    if (str_data.indexOf("off") != -1) // ถ้า payload คือคำว่า off ให้ LED ดับ
      digitalWrite(LEDPIN, HIGH);
  }
}
/*
  connect_server คือ Function สำหรับเชื่อมต่อกับ MQTT Server และเมื่อเชื่อมต่อสำเร็จแล้ว จะส่งชื่อ Topic ที่ต้องการ Subscribe ไปยัง Server
*/
void connect_server()
{
  do
  {
    Serial.println(F("Connect Server"));
    Serial.println(F("wait connect"));
    if (mqtt.DisconnectMQTTServer()) //สั่งให้ตัดการเชื่อมต่อกับ MQTT Server
    {
      mqtt.ConnectMQTTServer(MQTT_SERVER, MQTT_PORT); // เชื่อมต่อกับ MQTT Server
    }
    delay(500);
    Serial.println(mqtt.ConnectState()); // แสดงสถานะการเชื่อมต่อ
  }
  while (!mqtt.ConnectState()); //ตรวจเช็คและวนรอบเชื่อมต่อกับ MQTT Server จนสำเร็จจึงหลุด Loop
  Serial.println(F("Server Connected"));
  // Login โดยส่ง username , password
  unsigned char ret = mqtt.Connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD);
  Serial.println(mqtt.ConnectReturnCode(ret));  //แสดงสถาณะการ login ที่ Server ตอบกลับมา
  if (ret == 0) // ถ้าหาก login สำเร็จ
  {
    mqtt.Subscribe(SUBSCRIBE);        // ส่ง Topic ที่ต้องการ Subscribe ไปยัง Server
  }
}

void loop()
{
  //ดึงค่าเวลาที่ Arduino ทำงานตั้งแต่เริ่มทำงานจนถึงปัจจุบัน (millisecond)
  unsigned long currentMillis = millis();
  /*เปรียบเทียบว่า ค่าเวลาปัจจุบัน เมื่อลบกับ ค่าเวลาก่อนหน้า มาค่ามากกว่าหรือเท่ากับ ค่าเวลาที่ตั้งเอาไว้หรือไม่
    ผลของเงื่อนไขนี้ก็คือ จะมีการทำงานตามเงื่อนไข(true) ในทุกๆคาบเวลาที่กำหนดไว้ในตัวแปล intervalmqtt (ในที่นี้คือ ทำงานทุกๆ 2 วินาที ซึ่งเป็นค่าที่ dth22 กำหนดเอาไว้ว่าควรอ่าค่าจาก Sensor ห่างกันอย่างน้อย 2 วินาที)
  */
  if (currentMillis - previousmqtt >= intervalmqtt)
  {
    float h = dht.readHumidity();  //อ่านค่า humidity จาก Sensor
    float t = dht.readTemperature(); // อ่านค่า temp จาก sensor
    String str = String(t, 2);   // แปลงค่า temp จาก Float ให้อยู่ในรูป String แสดง ทศนิยม 2 ตำแหน่ง
    mqtt.Publish("Temp", str);   // Publish Topic Temp ไปยัง Server
    str = String(h, 2);  // แปลงค่า humidity จาก Float ให้อยู่ในรูป String แสดง ทศนิยม 2 ตำแหน่ง
    mqtt.Publish("Humi", str);   // Publish Topic Humi ไปยัง Server
    previousmqtt = currentMillis; // นำค่าเวลาปัจจุบัน ไปเก็บเป็นค่าเวลาก่อนหน้านี้
  }
  mqtt.MqttLoop();      // Function คอยตรวจจับเมื่อมีข้อมูลส่งมาจาก 3G
  if (!mqtt.ConnectState())   // ตรวจสอบว่ายังสามารถเชื่อมต่อกับ Server ได้อยู่หรือไม่
    connect_server();      //เชื่อมต่อกับ Server
}
