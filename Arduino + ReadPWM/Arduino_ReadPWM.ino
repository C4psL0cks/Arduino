//BOARD  PWM PINS  PWM FREQUENCY
//Uno, Nano, Mini
//
//3, 5, 6, 9, 10, 11
//
//490 Hz (pins 5 and 6: 980 Hz)

int ch1 ; // to keep channel values

void setup()
{
  pinMode(9, INPUT); // set out input pin as such
  Serial.begin(9600);
}

void loop()
{
  ch1 = pulseIn(9, HIGH, 25000); // Read the pulse width of the channel
  Serial.print(“channel data : “); // print value of
  Serial.println(ch1) ; // the channel

  delay(100);
}
