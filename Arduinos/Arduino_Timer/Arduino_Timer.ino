
//Blink without Delay skeleton
//4 examples demonstrated


//LED wiring options
//=============================================
//Depending which way your LEDs are wired, uncomment the next line.
//#define PlusEqualsON

#ifdef PlusEqualsON
//wired so +5V turns LED ON
#define ledON  HIGH
#define ledOFF LOW
//=========================
#else
//wired so +5V turns LED OFF
#define ledON  LOW
#define ledOFF HIGH
//=========================
#endif

//switch wiring options
//=============================================
//Depending which way your switches are wired, uncomment the next line.
#define PushEqualsLOW

#ifdef PushEqualsLOW
//pushing the switch makes pin LOW
#define Pushed   LOW
#define Released HIGH
//=========================
#else
//pushing the switch makes pin HIGH
#define Pushed   HIGH
#define Released LOW
//=========================
#endif

//=============================================
unsigned long currentMillis;
unsigned long pin13Millis;
unsigned long pin12Millis;
unsigned long pin11Millis;
unsigned long SwitchMillis;

//if these are not changed in the sketch, they can be const
unsigned long debounceMillis = 100UL;     //100ms
unsigned long ledOnTime      = 5*1000UL;  //5 seconds

byte laststartSwitchState    = HIGH;
byte buttonState             = HIGH;
byte counter                 = 0;

//the following are enable/disable flags
//some of these might not be used in this sketch
boolean flag13 = true;
boolean flag12 = true;
boolean flag11 = true;
boolean flag10 = true;

const byte startSwitch = 2; //pushed = LOW
const byte testSwitch  = 3; //pushed = LOW

//**********************************************************************

void setup()
{
  Serial.begin(9600);
  
  digitalWrite(13,ledOFF);
  pinMode(13, OUTPUT);

  digitalWrite(12,ledOFF);
  pinMode(12, OUTPUT);

  digitalWrite(11,ledOFF);
  pinMode(11, OUTPUT);
  
  digitalWrite(10,ledOFF);
  pinMode(10, OUTPUT);

  pinMode(startSwitch, INPUT_PULLUP); //pushed = LOW
  pinMode(testSwitch,  INPUT_PULLUP); //pushed = LOW

} //  >>>>>>>>>>>>>> E N D  O F  s e t u p ( ) <<<<<<<<<<<<<<<<<

void loop()
{
  //save the current time
  currentMillis = millis();

  //************************************* E x a m p l e  1
  //toggle pin 13 every 200mS
  //has 200ms or more gone by?
  if (currentMillis - pin13Millis >= 200UL)
  {
    //code here runs every 200ms
    //get ready for next iteration
    pin13Millis = pin13Millis + 200UL;
    //toggle pin 13
    digitalWrite(13,!digitalRead(13));
  }

  //************************************* E x a m p l e  2
  //at power up, pin 12 LED goes ON, after 3 seconds goes OFF and stays OFF
  //could be used as a powerup reset signal
  if (flag12 == true && currentMillis - pin12Millis <= 3000UL)
  {
    //code here runs for 3 seconds after power up, then stops
    digitalWrite(12,ledON);
  }
  else
  {
    digitalWrite(12,ledOFF);
    //disable further pin 12 control
    flag12 = false;
  }

  //************************************* E x a m p l e  3
  //if testSwitch is pushed and released
  //pin 11 LED goes ON for 5 seconds, then goes OFF
  buttonState = digitalRead(testSwitch);
  
  //are we are allowed to check the switch and is it pressed?
  if(flag11 == true && buttonState == Pushed)
  {    
    //enable timing of LED on pin 11
    flag11 = false; //false --> timing is enabled
    //turn LED ON
    digitalWrite(11,ledON);
    //record the time LED turned ON
    pin11Millis = currentMillis;
  }
    
  //are we allowed and is it time to control pin 11
  if (flag11 == false && currentMillis - pin11Millis >= ledOnTime)
  {
    //if enabled, code here runs after ledOnTime ms goes by
    digitalWrite(11,ledOFF);
    //allow switch press detection again
    flag11 = true; //true --> switch monitoring is enabled
  }

  //************************************* E x a m p l e  4
  //is it time to check the switches?
  //in particular, pushing startSwitch will turn ON/OFF (toggle) an output pin 10
  //is it time to check the switches
  if (currentMillis - SwitchMillis >= debounceMillis)
  {
    //code here runs every debounceMillis ms
    //get ready for the next iteration
    SwitchMillis += debounceMillis;
    //go and check the switches
    checkSwitches();    
  }

  //*********************************
  //put other non-blocking stuff here
  //*********************************

} //  >>>>>>>>>>>>>> E N D  O F  l o o p ( ) <<<<<<<<<<<<<<<<<


//======================================================================
//                      F U N C T I O N S
//======================================================================


//****************** c h e c k S w i t c h e s ( ) *********************
//switches are checked every debounceValue milli seconds
//no minimum switch press time is validated with this code (i.e. No glitch filter)
void checkSwitches()  
{
  //re-usable for all the switches  
  boolean thisState;    

  //************************************* E x a m p l e  Push ON push OFF (toggle)  
  //check if this switch has changed state
  thisState = digitalRead(startSwitch);
  if (thisState != laststartSwitchState)
  {  
    //update the switch state
    laststartSwitchState = thisState;  

    //this switch position has changed so do some stuff

    //"HIGH condition code"
    //switch went from LOW to HIGH
    if(thisState == HIGH)        
    {
      //Do some HIGH switch stuff here
    }

    //"LOW condition code"
    //switch went from HIGH to LOW
    else                          
    {
      //Do some LOW switch stuff here  
      digitalWrite(10, !digitalRead(10));
      //print number of pushes
      counter++;
      Serial.println(counter);
    }

  } //END of startSwitch code

  //*****************************************  
  //similar code for other switches goes here
  //*****************************************  

} //END of checkSwitches()

//**********************************************************************

//======================================================================
//                      E N D  O F  C O D E
//======================================================================
