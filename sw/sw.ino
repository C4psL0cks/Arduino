int sw = 13;
int counter;
int Last_sate;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sw, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  int reads = digitalRead(sw);
  if (reads == 0 && Last_sate == 1) {
    counter += 1;
    if (counter == 1) {
      counter = 0;
    }
  }
  Last_sate = reads;
  Serial.println(counter);
  delay(100);

}
