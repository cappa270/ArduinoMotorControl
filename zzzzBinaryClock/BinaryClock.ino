/* Binary Clock

    Keeps time and displays the time in binary using LEDs
  
    Circuit:
    *LEDs attached to pins 
  
    Created 2012
    by Mitch Fynaardt 
    mod-ed 25 Feb 2012
    Private domain*/
  
// constants won't change. Used here to
// set pin numbers:
// minutes pins
const int ledPin13 = 13;
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;
const int ledPin9 = 9;
const int ledPin8 = 8;
  
// seconds pins
const int ledPin7 = 7;
const int ledPin6 = 6;
const int ledPin5 = 5;
const int ledPin4 = 4;
const int ledPin3 = 3;
const int ledPin2 = 2;

// Variables will change:
long previousSeconds = 0;          // store last time LED was updated
int minutesCounter = 0;
int secondsCounter = 0;
int pin2State = LOW;              // initialize pin states to LOW
int pin3State = LOW;
int pin4State = LOW;
int pin5State = LOW;
int pin6State = LOW;
int pin7State = LOW;
int pin8State = LOW;
int pin9State = LOW;
int pin10State = LOW;
int pin11State = LOW;
int pin12State = LOW;
int pin13State = LOW;

long interval = 1;      // define the interval to be 1 second in length

void setup() {
    // set the digi pins as output:
    // minutes pins
    pinMode(ledPin13, OUTPUT);
    pinMode(ledPin12, OUTPUT);
    pinMode(ledPin11, OUTPUT);
    pinMode(ledPin10, OUTPUT);
    pinMode(ledPin9, OUTPUT);
    pinMode(ledPin8, OUTPUT);
    
    // seconds pins
    pinMode(ledPin7, OUTPUT);
    pinMode(ledPin6, OUTPUT);
    pinMode(ledPin5, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    
    // initialize pin states
    pin2State = LOW;
    pin3State = LOW;
    pin4State = LOW;
    pin5State = LOW;
    pin6State = LOW;
    pin7State = LOW;
    pin8State = LOW;
    pin9State = LOW;
    pin10State = LOW;
    pin11State = LOW;
    pin12State = LOW;
    pin13State = LOW;
     
  }
  
  void loop() {
    // constantly running code
   unsigned long currentSeconds = millis()/1000;
   if(currentSeconds - previousSeconds >= interval) {
     previousSeconds = currentSeconds;
     secondsCounter++;
        
       /* pin2State = switchPinState(pin2State);
        pin3State = switchPinState(pin3State);
        pin4State = switchPinState(pin4State);
        pin5State = switchPinState(pin5State);
        pin6State = switchPinState(pin6State);
        pin7State = switchPinState(pin7State);
        pin8State = switchPinState(pin8State);
        pin9State = switchPinState(pin9State);
        pin10State = switchPinState(pin10State);
        pin11State = switchPinState(pin11State);
        pin12State = switchPinState(pin12State);
        pin13State = switchPinState(pin13State);*/
        
         // if on an even minute evaluate minute LEDs
        if( currentSeconds % 60 <= 1) {
            minutesCounter++;              //add a minute to the count to use as a multiplier         
            
            if( minutesCounter == 60) {
               minutesCounter = 0;
              
               /* reinitialize minutes lights */
               
            } else {
              // eigth pin state
              pin8State = switchPinState(pin8State);
              
              // 9th pin state
              if( minutesCounter % 2 == 0) {
                  pin9State = switchPinState(pin9State);
              }
              // 10th pin state
              if( minutesCounter % 4 == 0) {
                pin10State = switchPinState(pin10State);
              }
              // 11th pin state
              if( minutesCounter % 8 == 0) {
                  pin11State = switchPinState(pin11State);
              }
              // 12th pin state
              if( minutesCounter % 16 == 0) {
                  pin12State = switchPinState(pin12State);
              }
              // sixth pin state
              if( minutesCounter % 32 == 0) {
                  pin13State = switchPinState(pin13State);
              }
              // reset seconds counter pins for next minute
            
            //possible edit
              pin2State = LOW;
              pin3State = LOW;
              pin4State = LOW;
              pin5State = LOW;
              pin6State = LOW;
              pin7State = LOW;  
          }
        } else {
            if( secondsCounter >= 60) {
              secondsCounter = 1;
            }
              
              // lowest pin state
              pin2State = switchPinState(pin2State);
        
              // second pin state
              if( currentSeconds > 0) {
                if( secondsCounter % 2 <= 1) {
                    pin3State = switchPinState(pin3State);
                }
                // third pin state
                if( secondsCounter % 4 <= 1) {
                    pin4State = switchPinState(pin4State);
                }
                // fourth pin state
                if( secondsCounter % 8 <= 1) {
                    pin5State = switchPinState(pin5State);
                }
                // fifth pin state
                if( secondsCounter % 16 <= 1) {
                    pin6State = switchPinState(pin6State);
                }
                // sixth pin state
                if( secondsCounter % 32 <= 1) {
                    pin7State = switchPinState(pin7State);
                }
              }
            
        }
              
          // set each LED with the ledState of each variable:
          digitalWrite(ledPin2, pin2State);
          digitalWrite(ledPin3, pin3State);
          digitalWrite(ledPin4, pin4State);
          digitalWrite(ledPin5, pin5State);
          digitalWrite(ledPin6, pin6State);
          digitalWrite(ledPin7, pin7State);
          digitalWrite(ledPin8, pin8State);
          digitalWrite(ledPin9, pin9State);
          digitalWrite(ledPin10, pin10State);
          digitalWrite(ledPin11, pin11State);
          digitalWrite(ledPin12, pin12State);
          digitalWrite(ledPin13, pin13State);
    }
  }
  
int switchPinState(int pinState) {
    if(pinState == LOW) {
       return HIGH;
    }
    else {
        return LOW;
    }
}
  

    
