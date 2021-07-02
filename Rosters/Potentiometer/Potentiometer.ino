const int CS1 = 2;
const int INC1 = 3;
const int UD1 = 4;

const int CS2 = 7;
const int INC2 = 8;
const int UD2 = 9;

void setup() {
  Serial.begin(115200);
  pinMode(CS1, OUTPUT);
  pinMode(INC1, OUTPUT);
  pinMode(UD1, OUTPUT);
  digitalWrite(CS1, HIGH);
  digitalWrite(INC1, HIGH);
  digitalWrite(UD1, HIGH);

  pinMode(CS2, OUTPUT);
  pinMode(INC2, OUTPUT);
  pinMode(UD2, OUTPUT);
  digitalWrite(CS2, HIGH);
  digitalWrite(INC2, HIGH);
  digitalWrite(UD2, HIGH);
}

void loop() {
  //  for (int i = 0; i <= 100; i += 10) {
  //    setResistance1(i); // กำหนดค่า % ความต้านทาน
  //    Serial.println(i);
  //    delay(1000);
  //  }

  //  for (int i = 0; i <= 100; i += 10) {
  //    setResistance2(i); // กำหนดค่า % ความต้านทาน
  //    Serial.println(i);
  //    delay(1000);
  //  }

  int S1 = 50;
  setResistance1(S1); // กำหนดค่า % ความต้านทาน
  Serial.println("P1:" + String(S1));

  int S2 = 50;
  setResistance2(S2); // กำหนดค่า % ความต้านทาน
  Serial.println("P2:" + String(S2));

  delay(1000);
}

void setResistance1(int percent) {
  digitalWrite(UD1, LOW);
  digitalWrite(CS1, LOW);
  for (int i = 0; i < 100; i++) {
    digitalWrite(INC1, LOW);
    delayMicroseconds(1);
    digitalWrite(INC1, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(UD1, HIGH);
  for (int i = 0; i < percent; i++) {
    digitalWrite(INC1, LOW);
    delayMicroseconds(1);
    digitalWrite(INC1, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(CS1, HIGH);
}

void setResistance2(int percent) {
  digitalWrite(UD2, LOW);
  digitalWrite(CS2, LOW);
  for (int i = 0; i < 100; i++) {
    digitalWrite(INC2, LOW);
    delayMicroseconds(1);
    digitalWrite(INC2, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(UD2, HIGH);
  for (int i = 0; i < percent; i++) {
    digitalWrite(INC2, LOW);
    delayMicroseconds(1);
    digitalWrite(INC2, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(CS2, HIGH);
}
