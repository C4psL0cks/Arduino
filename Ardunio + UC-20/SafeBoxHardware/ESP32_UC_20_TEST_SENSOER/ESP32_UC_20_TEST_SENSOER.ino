#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
#define oneWireBus 18
#define water1 26
#define water2 27
#define motion 25
#define car 33

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

int water_sensor_1 = 0;
int water_sensor_2 = 0;
int motion_sensor = 0;
int car_sensor = 0;
float temperatureC = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();
  pinMode(water1, INPUT);
  pinMode(water2, INPUT);
  pinMode(motion, INPUT);
  pinMode(car, INPUT);
  Serial.println("Start.....");

}

void loop() {
  // put your main code here, to run repeatedly:


  water_sensor_1 = digitalRead(water1);
  Serial.println("Water1:" + String(water_sensor_1));
  water_sensor_2 = digitalRead(water2);
  Serial.println("Water2:" + String(water_sensor_2));
  motion_sensor = digitalRead(motion);
  Serial.println("motion:" + String(motion_sensor));
  car_sensor = digitalRead(car);
  Serial.println("car:" + String(car_sensor));
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);
  Serial.println(String(temperatureC) + "ºC");
  Serial.println("---------------------------------------------");

  delay(500);




}
