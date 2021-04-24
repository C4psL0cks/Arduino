
void sonic5(void*) {

  const int trigPin = 10;
  const int echoPin = 11;
  int led = 12;

  VOID SETUP() {
    Serial.begin(9600);
  }
  VOID LOOP() {
    long duration5, cm5;
    pinMode(led, OUTPUT);
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration5 = pulseIn(echoPin, HIGH);
    cm5 = duration5 / 29 / 2;
    if (cm5 < 8) {
      checkstatus5 = 1; //ช่อง 2 ไม่ว่าง
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

    } else {
      checkstatus5 = 0; //ช่อง 2 ว่าง
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    delay(100);
  }
}
