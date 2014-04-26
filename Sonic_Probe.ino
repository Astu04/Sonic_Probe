//It it returns stk500_recv, press restart when uploading. 

// Super Probe
//Pro Mini, each output is (almost) 5V and a max of 40mAh, 
//it would be nice to reduce the individual stress of each pin
//to at least 20mAh. 

// Used tutorials/libraries: 
// http://hackaday.com/2013/11/20/primer-tutorials-for-arduino-ir-remote-cloning-and-keyboard-simulation/ 
// http://arduino.cc/en/Tutorial/InputPullupSerial 
// http://arduino.cc/en/Tutorial/buttonStateChange 
// https://code.google.com/p/clickbutton/
// http://www.instructables.com/id/TV-Volume-Loudness-Guard-using-Arduino/?ALLSTEPS
// http://www.righto.com/2010/03/understanding-sony-ir-remote-codes-lirc.html
// https://learn.sparkfun.com/tutorials/ir-communication/transmitting-ir-example
// https://learn.adafruit.com/tv-b-gone-kit/design-notes
// https://github.com/shirriff/Arduino-IRremote
// https://learn.adafruit.com/ir-sensor
//
//Functions: 
// 0 - Go to sleep
// 1 - Torch (Short -> toggle, long -> stay on until no pressed)
// 2 - Laser (Short -> toggle, long -> stay on until no pressed)
// 3 - Turn on the class projector (Mitsubishi), 
//     (Short -> toggle, long -> stay on until no pressed)
//     Also, turn on my TV and switch to AV (TDT) 
//     A delay for 5 seconds after executing where I can 
//     press again buttonB for the same codes to send, in case
//     the device hasn't powered up because I wasn't pointing
//     properly
//     Also, it would be nice if I get the codes for all projecors
// 4 - TV-B-Gone
// 5 - Just sound
// 6 - Auto go to sleep

// LIBRARIES 
#include <ClickButton.h> // Buttons, many functions. Very useful.
//#include "main.h" //TVB related
//#include <avr/pgmspace.h> //TVB related
#include <avr/sleep.h>
#include <IRremote.h>


#define REPEAT_TX         3 // how many times to transmit the IR remote code
#define IRLED        9      // LED used to blink when volume too high
			  // No cambiar, tiene que ser el 9 sí o sí, limitaciones. 
IRsend irsend; // instantiate IR object

// #include <avr/sleep.h> // Low consumption, to implement
// CONSTANTS AND VARIABLES
const int buttonPin1 = 2;
ClickButton button1(buttonPin1, HIGH, CLICKBTN_PULLUP);
const int buttonPin2 = 3;
ClickButton button2(buttonPin2, HIGH, CLICKBTN_PULLUP);
//const int buttonA = 2; // To switch among the variables
//const int buttonB = 3; // To activate, it could be short or long
// click, see the table at the start of the code for +info. 
// In an update, I'll use an ear bud as a speaker, with a low 
// resistor. I think will be able to get a nice enough sounds.
// I still don't know how am I going to do the sound. 
//const int speaker = 4;
const int TORCH = 10;
const int LASER = 12;
//const int debug = 13;
// Variables from the buttons:
int button1PushCounter = 0;   // counter for the number of 
                              //button presses
int button1State = 0;         // current state of the button
int lastbutton1State = 0;     // previous state of the button
int button2State = 0;         // current state of the button
int lastbutton2State = 0;     // previous state of the button
// int start = 0;             // It loops for 10 seconds&sleep 
                              //or
    
                             // it goes to the desired function

void setup()
{
  Serial.begin(9600);
  Serial.println("Start void setup");
  Serial.println("Hello world.");
  pinMode(buttonPin1,INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(IRLED, OUTPUT);
  pinMode(TORCH, OUTPUT);
  pinMode(LASER, OUTPUT); 

  
  // Things from clickbutton.h
          // pinMode(buttonB, INPUT_PULLUP);  
      // Setup button timers (all in milliseconds / ms)
      // (These are default if not set, but changeable 
      // for convenience)
  button2.debounceTime   = 20;   // Debounce timer in ms
  // Multiclick isn't used? Just longclick. 
  //buttonB.multiclickTime = 250;  // Time limit for multi clicks
  button2.longClickTime  = 1000;  // time until "held-down clicks" 
                                  //register

}

void loop()
{
// Initilizing
Serial.println("Start_init");

start: // Here we have to introduce the check for A or B. 
       // Still no idea how that would be done. 
 lastbutton2State == button2State; 
 for (int i=0; i<=100;i++){
    button2State = digitalRead(buttonPin2);  
    if (button2State != lastbutton2State) {
      // if the state has changed, goto function
      
      if (button2State == LOW) {
          // if the current state is LOW then the button
          // went from off to on:

        lastbutton2State == button2State; 
        if (button1PushCounter == 0) {goto sleep;}
        if (button1PushCounter == 1) {goto torch;}
        if (button1PushCounter == 2) {goto laser;}
        if (button1PushCounter == 3) {goto turnonprojector;}
        if (button1PushCounter == 4) {goto tvbgone;}
        if (button1PushCounter == 5) {goto onlysound;}   
        if (button1PushCounter >> 5) {goto sleep;} 
      }
      else { // Nothing in else, keeps reading to next       

      }
      button1State = digitalRead(buttonPin1);  
      if (button1State != lastbutton1State) {
          // if the state has changed, increment the counter
          if (button1State == LOW) {
             // if the current state is LOW then the button
             // went from off to on:
             lastbutton1State == button1State;
             button1PushCounter++;
          }
          else {  // Nothing in else, keeps reading to next     
          }
      } 
 
  delay(100);
 }
 } 
 
       // if the current state is HIGH and the button has
       // looped here for 10 seconds, if buttonA no pressed, 
 
      // go to sleep
  goto sleep;
  

 
  // 1 - Torch (Short -> toggle, long -> stay on until no pressed)
torch:
                                       digitalWrite(13, HIGH);
    digitalWrite(TORCH, HIGH);
    delay(500);
    
    // read the pushbutton input pin
  button2State = digitalRead(buttonPin2);  
      if (button2State == LOW) { // if it's pressed
      // it goes into "on until release mode"
      // meter un loop aquí que de vueltas hasta que 
      // se deje de presionar (sea high)  
      
      for (int i=0; i<=6000;i++){
        if (button2State == HIGH) { 
          goto sleep;  //the button has been unpressed so it
                       //goes to sleep because it has finished
                       //it's function
        }
        delay(50);
      } // If it is still pressed, it'll loop until it's not.      
      }
      else { // If it has been released, toggle mode 
             // Wait until buttonBState != lastbuttonBState
             // Loop until buttonBState = LOW -> button has 
             // been pressed     
      for (int i=0; i<=6000;i++){
        if (button2State == LOW) { 
          digitalWrite(TORCH, LOW);
          goto sleep;
        }
        delay(50);
      }
      goto sleep;
    }    
  goto sleep;  
  // 2 - Laser (Short -> toggle, long -> stay on until
  // no pressed)
laser:
  // 3 - Turn on the class projector (Mitsubishi), 
turnonprojector:
  //     (Short -> toggle, long -> stay on until no pressed)
 digitalWrite(IRLED,HIGH);  // LED on
    delay(100);

//Turn off Sony Trinitron TV: 
    for (int txCount = 0; txCount < REPEAT_TX; txCount++) { // how many times to transmit the IR remote  code
      irsend.sendSony(0xA90 , 12); // Change to match your remote protocol
    }                              // Syntax: (REMOVE_CODE, REMOTE_BIT)
    delay(1000);

//Turn on Sony Trinitron TV&Switch to AV (TDT): 
    for (int txCount = 0; txCount < REPEAT_TX; txCount++) { // how many times to transmit the IR remote  code
      irsend.sendSony(0x10 , 12); // Change to match your remote protocol
    }                             // Syntax: (REMOVE_CODE, REMOTE_BIT)
    delay(2000); //Have to wait a little before switching to AV
    for (int txCount = 0; txCount < REPEAT_TX; txCount++) { // how many times to transmit the IR remote  code
      irsend.sendSony(0xA50 , 12); // Change to match your remote protocol
     }                             // Syntax: (REMOVE_CODE, REMOTE_BIT)
     
     
      delay(100);
 goto sleep;  
  
  // 4 - TV-B-Gone
  
tvbgone: 
  // 5 - Just sound
onlysound:
  // 6 - Auto go to sleep 
  // When 6 it means more than n-1. 
  // After sleep, the device shall start again from 'start:'
sleep: 
sleepNow();
}

void sleepNow()
{
  set_sleep_mode(buttonPin1);                    // sleep mode is set here

  sleep_enable();                             // enables the sleep bit in the mcucr register

  attachInterrupt(0, wakeUpNow, LOW);         // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW

  sleep_mode();                               // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE ON WAKE

  sleep_disable();                            // first thing after waking, disable sleep

  detachInterrupt(0);                         // disables int 0 as the wakeupnow code will
                                              // not be executed during normal runtime
}

void wakeUpNow()
{
  // any needed wakeup code can be placed here
}


