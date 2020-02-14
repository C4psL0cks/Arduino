#include <SimpleTimer.h>
SimpleTimer timer;

const int  trigpin = 2;
const int  echopin = 3;
const int  redledpin = 4;
const int  blueledpin = 5;

#define redledon() digitalWrite(redledpin, HIGH)
#define redledoff() digitalWrite(redledpin, LOW)
#define blueledon() digitalWrite(blueledpin, HIGH)
#define blueledoff() digitalWrite(blueledpin, LOW)

bool blinkEnble = false;

void SensorRelease(void) {
  blueledoff();
}

void SensorHoldRelease(void) {
  blinkEnble = false;
}

void SensorTrig(void) {
  blueledon();
}

void SensorHold(void) {
  blinkEnble = true;
  blueledoff();
}
void PingSensor(void) {
#define sensor_normal true
#define sensor_active false
  static unsigned long scan;
  static unsigned int wait;
  static enum {ssnact = 0, ssact, ssrep, sswrel} sensor_state;
  static bool ssbit, sshold, sensor;

  long duration, cm;

  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH);

  cm = (duration / 2) / 2;

  Serial.print(cm);

  if (cm < 10) {
    sensor = sensor_active;
  }
  else {
    sensor = sensor_normal;
  }

  if (sensor == sensor_normal) {
    if (ssbit == true) {
      ssbit = false;
      SensorRelease();
    }
    if (sshold == true) {
      sshold = false;
      SensorHoldRelease();
    }
    sensor_state = ssnact;
    return;
  }

  if (sensor == sensor_active && sensor_state == ssnact) {
    sensor_state = ssact;
    return;
  }

  if (sensor == ssact) {
    ssbit = true;
    wait = 10;
    sensor_state = ssrep;
    SensorTrig();
    return;
  }

  if (sensor == ssrep) {
    if (wait) {
      wait--;
      return;
    }
    ssbit = false;
    sshold = true;
    sensor_state = sswrel;
    SensorHold();
    return;
  }
}

void BlinkLed(void) {
  static unsigned long scan;
  static bool fbit;
  if (blinkEnble == true || fbit == false) {
    if (fbit == true && millis() - scan >= 200) {
      scan = millis();
      fbit = false;
      redledon();
    }
    if (fbit == false && millis() - scan >= 30) {
      scan = millis();
      fbit = true;
      redledoff();
    }
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(redledpin, OUTPUT);
  pinMode(blueledpin, OUTPUT);

  pinMode(trigpin, OUTPUT);
  digitalWrite(trigpin, LOW);
  pinMode(echopin, INPUT_PULLUP);
  timer.setInterval(100L, PingSensor);

}

void loop() {
  timer.run();
  BlinkLed();

}
