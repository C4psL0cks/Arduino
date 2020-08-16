
// 0-25v voltage sensor is connected with the analog pin A0 of the arduino
int Vsensor = A0;

//For 0-25v voltage sensor
float correctionfactor = 5.14;
float vout = 0.0;
float vin = 0.0;

// two resistors 30K and 7.5k ohm
float R1 = 4700.0;
float R2 = 2200.0;
int value = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  value = analogRead(Vsensor);
  vout = (value * correctionfactor) / 1024.0; 
  //Serial.println(value);
  vin = vout / (R2 / (R1 + R2));
  //vin = vin - correctionfactor;
  Serial.print("INPUT V= ");
  Serial.println(vin, 2);
  delay(100);
}
