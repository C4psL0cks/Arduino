
// 0-25v voltage sensor is connected with the analog pin A0 of the arduino
int Vsensor = A0;

//For 0-25v voltage sensor
float correctionfactor = 5.0;
float vout = 0.0;
float vin = 0.0;

// two resistors 30K = 30000 and 7.5k = 7500 ohm
float R1 = 4700.0; //4.7 k
float R2 = 2200.0; //2.2 k
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
