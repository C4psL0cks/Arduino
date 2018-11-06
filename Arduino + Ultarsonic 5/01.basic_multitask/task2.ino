
void sonic2(void*) {

  const int trigPin = 4;
  const int echoPin = 5;
  int led = A1;

  VOID SETUP() {
    Serial.begin(9600);
  }
  VOID LOOP() {
    long duration2, cm2;
    pinMode(led, OUTPUT);
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration2 = pulseIn(echoPin, HIGH);
    cm2 = duration2 / 29 / 2;
    if (cm2 < 8) {
      checkstatus2 = 1; //ช่อง 2 ไม่ว่าง
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    } else {
      checkstatus2 = 0; //ช่อง 2 ว่าง
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    delay(100);
  }
}
