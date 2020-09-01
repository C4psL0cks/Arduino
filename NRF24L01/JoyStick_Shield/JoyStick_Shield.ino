//MOSI = D11
//CSN = D10
//VCC = 3V
//MISO = D12
//SCK = D13
//CE = D9

const byte PIN_ANALOG_X = 0;
const byte PIN_ANALOG_Y = 1;
const byte PIN_A = 2;
const byte PIN_B = 3;
const byte PIN_C = 4;
const byte PIN_D = 5;
const byte PIN_E = 6;
const byte PIN_F = 7;
const byte PIN_K = 8;


void setup() {
  Serial.begin(115200);

  pinMode(PIN_A, INPUT);
  digitalWrite(PIN_A, HIGH);

  pinMode(PIN_B, INPUT);
  digitalWrite(PIN_B, HIGH);

  pinMode(PIN_C, INPUT);
  digitalWrite(PIN_C, HIGH);

  pinMode(PIN_D, INPUT);
  digitalWrite(PIN_D, HIGH);

  pinMode(PIN_E, INPUT);
  digitalWrite(PIN_E, HIGH);

  pinMode(PIN_F, INPUT);
  digitalWrite(PIN_F, HIGH);

  pinMode(PIN_K, INPUT);
  digitalWrite(PIN_K, HIGH);
}

void loop() {
  Serial.print("x:");
  Serial.print(analogRead(PIN_ANALOG_X));
  Serial.print(" ");

  Serial.print("y:");
  Serial.print(analogRead(PIN_ANALOG_Y));
  Serial.print(" ");

  Serial.print("A:");
  Serial.print(digitalRead(PIN_A));
  Serial.print(" ");

  Serial.print("B:");
  Serial.print(digitalRead(PIN_B));
  Serial.print(" ");

  Serial.print("C:");
  Serial.print(digitalRead(PIN_C));
  Serial.print(" ");

  Serial.print("D:");
  Serial.print(digitalRead(PIN_D));
  Serial.print(" ");

  Serial.print("E:");
  Serial.print(digitalRead(PIN_E));
  Serial.print(" ");

  Serial.print("F:");
  Serial.print(digitalRead(PIN_F));
  Serial.print(" ");

  Serial.print("K:");
  Serial.print(digitalRead(PIN_K));
  Serial.print(" ");


  Serial.println();
}
