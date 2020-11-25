#include <AltSoftSerial.h>
AltSoftSerial altSerial;

// ACS712 Demo Sketch
//

// Select sensitivity for appropriate ACS712 version:
//int Sens = 185;           // Sensitivity in mV/A for the 5A version
//int Sens = 100;           // Sensitivity in mV/A for the 20A version
int Sens = 66;              // Sensitivity in mV/A for the 30A version

const int analogIn = A0;  // Analog input pin

int OffsetVoltage = 2500; // 0 Current offset Voltage in mV @ 0 amps

int RawValue = 0;         // Init result variables
double Voltage = 0;       //
double Amps = 0;          //

void setup() {

  Serial.begin(9600);
  while (!Serial);
  altSerial.begin(9600);
  altSerial.println("Hello World");
  Serial.println("\nConnected device ...version 1.0.0");
}

void loop() {

  //  RawValue = analogRead(analogIn);           // Read voltage from ASC712
  //  Voltage = (RawValue / 1023.0) * 5000;      // Convert to mV
  //  Amps = ((Voltage - OffsetVoltage) / Sens); // Convert to amps
  //
  //
  //  Serial.print("A/D Read Value = " );  // A/D read value
  //  Serial.print(RawValue);              //
  //
  //  Serial.print("\t mV = ");            // ACS712 Output voltage
  //  Serial.print(Voltage, 3);            //
  //
  //  Serial.print("\t Amps = ");          // Current measured
  //  Serial.println(Amps, 3);             //

  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available()) {
    altSerial.write(Serial.read());
  }

  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (altSerial.available()) {
    Serial.write(altSerial.read());
  }

  //send
  //  if (Serial.available()) {
  //    //altSerial.write(Serial.read());
  //    c = Serial.read();
  //    altSerial.println(c);
  //  }
  //  altSerial.println(random(0, 100));
  //  delay(1000);

}
