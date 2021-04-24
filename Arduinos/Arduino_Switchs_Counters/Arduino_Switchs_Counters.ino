#define SWITCH1 A2 // กำหนดขา

int counter = 0;
int Last_sate_sw;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SWITCH1, INPUT_PULLUP);
  digitalWrite(SWITCH1, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(SWITCH1) == 0 && Last_sate_sw == 1) {
    counter += 1;
    if (counter == 100) { // กำหนดค่าสูงสุดว่าเอาเท่าไหร่ เช่น สูงสุดไปเรื่อยๆ จนถึง 100
      counter = 0;
    }
  }
  Last_sate_sw = digitalRead(SWITCH1);
  Serial.println("COUNTER: " + String(counter));
}
