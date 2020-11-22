//long t;

int KeySwitchPin    = 4;  //สวิทช์กุญแจ
int LeftTurnSignal  = 5 ; //ไฟเลี้ยวซ้าย
int RightTurnSignal = 6 ; //ไฟเลี้ยวขวา
int HornSignal      = 7 ; //แตร
int LowBeamSignal   = 8 ; //ไฟต่ำ
int HighBeamSignal  = 9 ; //ไฟสูง

int AC = 0;
int FB = 0;
int RB = 0;

int keyState = 0;
int FrontBrake = 0;
int RearBrake = 0;
long Steering = 0;
float Accelerator = 0 ;

int KeySwitch   = 0 ; //สวิทช์กุญแจ
int LeftTurn    = 0 ; //ไฟเลี้ยวซ้าย
int RightTurn   = 0 ; //ไฟเลี้ยวขวา
int Horn        = 0 ; //แตร
int LowBeam     = 0 ; //ไฟต่ำ
int HighBeam    = 0 ; //ไฟสูง

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup()
{
  pinMode(KeySwitchPin, INPUT);
  pinMode(LeftTurnSignal, INPUT);
  pinMode(RightTurnSignal, INPUT);
  pinMode(HornSignal, INPUT);
  pinMode(LowBeamSignal, INPUT);
  pinMode(HighBeamSignal, INPUT);
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  Serial.begin(9600);
}

void loop()
{
  //t = millis();
  //Serial.print(t);
  AC = analogRead(A0);
  FB = analogRead(A5);
  RB = analogRead(A6);
  Accelerator = map(AC, 179, 860, 0, 100);
  FrontBrake = map(FB, 0, 10, 0, 100);
  RearBrake = map(RB, 0, 100, 0, 100);

  Steering = map(encoderValue, 0, 560, -280, 280);

  KeySwitch = digitalRead(KeySwitchPin);
  LeftTurn = digitalRead(LeftTurnSignal);
  RightTurn = digitalRead(RightTurnSignal);
  Horn = digitalRead(HornSignal);
  LowBeam = digitalRead(LowBeamSignal);
  HighBeam = digitalRead(HighBeamSignal);

  if (FrontBrake < 0) {
    FrontBrake = 0;
  }
  if (RearBrake < 0) {
    RearBrake = 0;
  }
  if (Accelerator < 0) {
    Accelerator = 0;
  }
  /*Serial.print("  Steering");
  Serial.print(Steering);
  Serial.print("  encoderValue");
  Serial.print(encoderValue);

  Serial.print("  AC");
  Serial.print(AC);
  Serial.print("  Accelerator");
  Serial.print(Accelerator);

  Serial.print("  FB");
  Serial.print(FB);
  Serial.print("  FrontBrake");
  Serial.print(FrontBrake);

  Serial.print("  RearBrake");
  Serial.print(RearBrake);
  Serial.print("  RB");
  Serial.print(RB);

  Serial.print("  KeySwitch");
  Serial.print(KeySwitch);
  Serial.print("  LeftTurn");
  Serial.print(LeftTurn);
  Serial.print("  RightTurn");
  Serial.print(RightTurn);
  Serial.print("  Horn");
  Serial.print(Horn);
  Serial.print("  LowBeam");
  Serial.print(LowBeam);
  Serial.print("  HighBeam");
  Serial.print(HighBeam);/*

  /*if(KeySwitch != keyState){
     // Key toggled
     keyState = KeySwitch;
     Serial.println("KEY_" + String(keyState));
    }
    else{
     String text = String(keyState) + '_' +String(Accelerator) + '_' + String(FrontBrake) + '_' + String(RearBrake)+ '_' + String(encoderValue)+ '_' + String(LeftTurn)+ '_' + String(RightTurn)+ '_' + String(Horn)+ '_' + String(LowBeam)+ '_' + String(HighBeam);
     Serial.println(text);
    }*/
  String text = String(KeySwitch) + '_' + String(Accelerator) + '_' + String(FrontBrake) + '_' + String(RearBrake) + '_' + String(Steering) + '_' + String(LeftTurn) + '_' + String(RightTurn) + '_' + String(Horn) + '_' + String(LowBeam) + '_' + String(HighBeam);
  Serial.println(text);
  /*
    else if(Accelerator > 0 || FrontBrake > 0 || RearBrake > 0){
    String text = String(Accelerator) + '_' + String(FrontBrake) + '_' + String(RearBrake)+ '_' + String(Steering);
    Serial.println(text);
    } */


  //delay(16.6666);
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}
