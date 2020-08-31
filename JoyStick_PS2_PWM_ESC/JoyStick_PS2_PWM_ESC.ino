#include <PS2X_lib.h>  //for v1.6
#include <avr/wdt.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define PWM 9      //output pin 3 //PWM
Servo ESC;   // create servo object to control the ESC
int pos = 1000 , Percent = 0, i = 0, PSS_LY_ = 0;
PS2X ps2x; // create PS2 Controller Class.
//With the pin (coil) states as an array of arrays
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize I2C addr 0x3c
  display.clearDisplay(); // clears the screen and buffer
  //  display.drawPixel(127, 63, WHITE);
  ESC.attach(PWM, 1000, 2000); // (pin, min pulse width, max pulse width in milliseconds)
  //ESC.attach(PWM);
  // ESC.writeMicroseconds(1000);
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  //arduino MEGA 2560
  error = ps2x.config_gamepad(52, 51, 53, 50, true, true); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  //arduino R3
  // error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }

}

void loop() {
  /* You must Read Gamepad to get new values
    Read GamePad and set vibration values
    ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
    if you don't enable the rumble, use ps2x.read_gamepad(); with no values

    you should call this at least once a second
  */



  if (error == 1) //skip loop if no controller found
    return;

  if (type == 2) { //Guitar Hero Controller

    ps2x.read_gamepad();          //read controller

    if (ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if (ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if (ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if (ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if (ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed");


    if (ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");

    if (ps2x.Button(UP_STRUM))         //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if (ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");


    if (ps2x.Button(PSB_START))                  //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");


    if (ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
    {
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
    }
  }

  else { //DualShock Controller

    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START))                  //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");


    if (ps2x.Button(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

      if (PSS_LY_ >= 1) {
        PSS_LY_ -= 1;
        Serial.print(PSS_LY_);
        pos = map(PSS_LY_, 127, 0, 1160, 1450);   // scale it to use it with the servo library (value between 0 and 180)
        Percent = map(PSS_LY_, 127, 0, 0, 100);   // scale it to use it with the servo library (value between 0 and 180)
      }

    }
    if (ps2x.Button(PSB_PAD_RIGHT)) {
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if (ps2x.Button(PSB_PAD_LEFT)) {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);

      if (PSS_LY_ <= 126) {
        PSS_LY_ += 1;
        Serial.print(PSS_LY_);
        pos = map(PSS_LY_, 127, 0, 1160, 1450);   // scale it to use it with the servo library (value between 0 and 180)
        Percent = map(PSS_LY_, 127, 0, 0, 100);   // scale it to use it with the servo library (value between 0 and 180)
      }
    }


    vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
    //how hard you press the blue (X) button

    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {



      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if (ps2x.Button(PSB_GREEN))
        Serial.println("Triangle pressed");
      if (ps2x.Button(PSB_L2) && ps2x.Button(PSB_R2) && ps2x.Button(PSB_L1) && ps2x.Button(PSB_R1))
        wdt_enable(WDTO_15MS); //Reset
    }


    if (ps2x.ButtonPressed(PSB_RED))            //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");

    if (ps2x.ButtonReleased(PSB_PINK))            //will be TRUE if button was JUST released
      Serial.println("Square just released");

    if (ps2x.NewButtonState(PSB_BLUE))           //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");


    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
      //        Serial.print("Stick Values:");
      //        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      //        Serial.print(",");
      //        Serial.print(ps2x.Analog(PSS_LX), DEC);
      //        Serial.print(",");
      //        Serial.print(ps2x.Analog(PSS_RY), DEC);
      //        Serial.print(",");
      //        Serial.print(ps2x.Analog(PSS_RX), DEC);
      //        Serial.print("-");
      pos = map(ps2x.Analog(PSS_LY), 127, 0, 1160, 1450);   // scale it to use it with the servo library (value between 0 and 180)
      Percent = map(ps2x.Analog(PSS_LY), 127, 0, 0, 100);   // scale it to use it with the servo library (value between 0 and 180)
      PSS_LY_ = ps2x.Analog(PSS_LY);
    }
    if (pos >= 0 && Percent >= 0) {
      // ESC.write(pos);    // Send the signal to the ESC
      ESC.writeMicroseconds(pos);
      Serial.println(pos);
      if (pos != i) {
        i = pos;
        display.clearDisplay();
      }
      // display.clearDisplay();
      display.display();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("PWM >> ");
      display.setTextSize(0);
      display.setCursor(0, 20);
      display.print("Microseconds : ");
      display.println(pos);
      display.setTextSize(4);
      display.setCursor(0, 32);
      display.print(Percent);
      display.println("%");
      display.display();
    }

  }

  delay(60);

}
