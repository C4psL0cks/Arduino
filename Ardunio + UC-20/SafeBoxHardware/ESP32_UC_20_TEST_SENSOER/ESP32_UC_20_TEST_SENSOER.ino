#include <OneWire.h>
#include <DallasTemperature.h>

#define oneWireBus 32
#define water1 34
#define water2 35
#define motion 25
#define car 33
#define buzzer 17
int water_sensor_1 = 0;
int water_sensor_2 = 0;
int motion_sensor = 0;
int car_sensor = 0;
float temperatureC = 0;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {

  Serial.begin(115200);
  sensors.begin();
  pinMode(water1, INPUT);
  pinMode(water2, INPUT);
  pinMode(motion, INPUT);
  pinMode(car, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH); //close
  Serial.println("Start.....");

}

void loop() {

  digitalWrite(buzzer, LOW); // open
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
