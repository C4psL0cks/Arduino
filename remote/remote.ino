int ch1; // ตัวแปรของ channel
int ch2;
void setup() {
  Serial.begin(9600);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
}

void loop() {

  ch1 = pulseIn(5, HIGH, 25000); // อ่านค่าความกว้างของสัญญาณของแต่ละ Channel
  ch2 = pulseIn(6, HIGH, 25000);
  Serial.print("Channel 1:"); // Print the value of
  Serial.println(ch1); // each channel
  Serial.print("Channel 2:");
  Serial.println(ch2);
  Serial.println("");
}
