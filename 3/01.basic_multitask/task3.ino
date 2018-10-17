
void sonic3(void*) {

  const int trigPin = 6;
  const int echoPin = 7;
  int led = A2;

  VOID SETUP() {
    Serial.begin(9600);
  }
  VOID LOOP() {
    long duration3, cm3;
    pinMode(led, OUTPUT);
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration3 = pulseIn(echoPin, HIGH);
    cm3 = duration3 / 29 / 2;
    if (cm3 < 8) {
      checkstatus3 = 1; //ช่อง 2 ไม่ว่าง
      digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
    } else {
      checkstatus3 = 0; //ช่อง 2 ว่าง
      digitalWrite(led, HIGH);    // turn the LED off by making the voltage LOW
    }
    delay(100);
  }
}
