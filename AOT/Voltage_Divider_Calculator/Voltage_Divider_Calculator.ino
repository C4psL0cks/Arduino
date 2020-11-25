void setup() {
  Serial.begin(9600);
}

void loop() {

  int value = analogRead(A0);
  float vout = (value * 5.14) / 1024.0; 
  //Serial.println(value);
  float vin = vout / (2200.0 / (4700.0 + 2200.0));
  Serial.print("INPUT V = ");
  Serial.println(vin, 2);
  delay(100);
}
