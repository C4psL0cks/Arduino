//BOARD  PWM PINS  PWM FREQUENCY
//Uno, Nano, Mini
//
//3, 5, 6, 9, 10, 11
//
//490 Hz (pins 5 and 6: 980 Hz)

int sensorPin = A0;

// select the pin for the LED
int ledPin = 9;

// variable from the sensor
int sensorValue;
int ledValue;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  ledValue = map( sensorValue, 0, 1023, 0, 255);
  Serial.println(ledValue);
  // fade LED
  analogWrite (ledPin , ledValue);
  delay(100);
}
