#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <LiquidCrystal_I2C.h> // เรียกใช้งานไลบารีควบคุมหน้าจอ LCD ขนาด 20 * 4
LiquidCrystal_I2C lcd(0x27, 20, 4); // กำหนด Address การเชื่อมต่อกับจอ LCD เป็น 0x27 ขนาดจอ 20 * 4


//  SoftwareSerial bt(tx_10, tx_11); // TX, RX
//  AltSoftSerial altSerial; // 9tx 8rx
//  Arduino D8 ASS RX - BT TX no need voltage divider
//  Arduino D9 ASS TX - BT TX through a voltage divider

//SoftwareSerial serial_connection(8, 9); //TX=pin 10, RX=pin 11
SoftwareSerial serial_connection(6, 7); //TX=pin 10, RX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
void setup()
{

  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
  lcd.begin();
  lcd.setCursor(0, 1);                      // set Cursor ให้เลื่อนไปที่ตำแหน่งที่ 2 แถวที่ 1
  lcd.print("GPS Start.....");        // เริ่มปริ้นคำว่า Hello, Welcome ตั้งแต่ ตำแหน่งที่ 2 ในแถวที่ 1 ตามที่กำหนด
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started
}

void loop()
{
  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("==========================================");
    Serial.println("Satellite Count:" + String(gps.satellites.value()));
    Serial.println("Latitude:" + String(gps.location.lat(), 6));
    Serial.println("Longitude:" + String(gps.location.lng(), 6));
    Serial.println("Speed MPH:" + String(gps.speed.mph()));
    Serial.println("Altitude Feet:" + String(gps.altitude.feet()));
    Serial.println("==========================================");

    //    lcd.setCursor(0, 0);  // clum,rows
    //    lcd.print("SCount:" + String(gps.satellites.value()));
    //    lcd.setCursor(0, 1);  // clum,rows
    //    lcd.print("Latitude:" + String(gps.location.lat(), 5));
    //    lcd.setCursor(0, 2);
    //    lcd.print("Longitude:" + String(gps.location.lng(), 5));
    //    lcd.setCursor(0, 3);
    //    lcd.print("Speed MPH:" + String(gps.speed.mph())); //gps.speed.mph() //gps.speed.value()
    //    //    delay(1000);
  }
}
