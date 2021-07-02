int Relay1 = 31;
int Relay2 = 33;
int Relay3 = 35;
int Relay4 = 37;

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
}
void loop() {

  for (int i = 31; i <= 37; i += 2) {
    digitalWrite(i, LOW);
    delay(1000);
  }

  for (int i = 31; i <= 37; i += 2) {
    digitalWrite(i, HIGH);
    delay(1000);
  }

  //  digitalWrite(Relay1, LOW);
  //  digitalWrite(Relay2, LOW);
  //  digitalWrite(Relay3, LOW);
  //  digitalWrite(Relay4, LOW);
  //  delay(1000);
  //  digitalWrite(Relay1, HIGH);
  //  digitalWrite(Relay2, HIGH);
  //  digitalWrite(Relay3, HIGH);
  //  digitalWrite(Relay4, HIGH);
  //  delay(1000);
}
