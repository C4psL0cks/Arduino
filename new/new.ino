int output_pin_zf = 4;  //ZF - Direction pwm
int output_pin_vr = 5;  //VR - speed
int speeds;
int val;

void setup() {

  Serial.begin(115200);
  pinMode(output_pin_zf, OUTPUT);
  pinMode(output_pin_vr, OUTPUT);

}

void loop() {

  val = analogRead(A0); // recive
  speeds = map(val, 50, 1023, 0, 170); // 0-255
  analogWrite(output_pin_vr, speeds); // Send PWM signal
  Serial.println(speeds);
  //
  //  digitalWrite(output_pin_zf, LOW); // back
  //  digitalWrite(output_pin_zf, HIGH); // front


}
