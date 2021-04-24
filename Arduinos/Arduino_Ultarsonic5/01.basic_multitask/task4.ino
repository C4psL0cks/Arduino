
void sonic4(void*) {

  const int trigPin = 8;
  const int echoPin = 9;
  int led = A3;

  VOID SETUP() {
    Serial.begin(9600);
  }
  VOID LOOP() {
    long duration4, cm4;
    pinMode(led, OUTPUT);
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration4 = pulseIn(echoPin, HIGH);
    cm4 = duration4 / 29 / 2;
    if (cm4 < 8) {
      checkstatus4 = 1; //ช่อง 2 ไม่ว่าง
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    } else {
      checkstatus4 = 0; //ช่อง 2 ว่าง
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    delay(100);
  }
}
