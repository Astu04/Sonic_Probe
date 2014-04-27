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
// 4 - TV-B-Gone
// 5 - Just sound
// 6 - Auto go to sleep

// LIBRARIES 
//#include "main.h" //TVB related
//#include <avr/pgmspace.h> //TVB related
#include <avr/sleep.h>
#include <IRremote.h>


#define REPEAT_TX         3 // how many times to transmit the IR remote code
#define IRLED        9      // LED used to blink when volume too high
			  // No cambiar, tiene que ser el 9 sí o sí, limitaciones. 
IRsend irsend; // instantiate IR object
const int buttonAPin = 2;     // the number of the pushbutton pin
const int buttonBPin = 3;     // the number of the pushbutton pin

// #include <avr/sleep.h> // Low consumption, to implement
// CONSTANTS AND VARIABLES


//const int buttonA = 2; // To switch among the variables
//const int buttonB = 3; // To activate, it could be short or long
// click, see the table at the start of the code for +info. 
// In an update, I'll use an ear bud as a speaker, with a low 
// resistor. I think will be able to get a nice enough sounds.
// I still don't know how am I going to do the sound. 
//const int speaker = 4;
const int TORCH = 10;
const int LASER = 12;
const int debug = 13;
int buttonAState = 0;         // variable for reading the pushbutton status
int buttonBState = 0;         // variable for reading the pushbutton status
int buttonAPushCounter = 0;   // counter for the number of 
                              //button presses

// Variables from the buttons:
//int button1PushCounter = 0;   // counter for the number of 
//                              //button presses
//int button1State = 0;         // current state of the button
//int lastbutton1State = 0;     // previous state of the button
//int button2State = 0;         // current state of the button
//int lastbutton2State = 0;     // previous state of the button
// int start = 0;             // It loops for 10 seconds&sleep 
                              //or
    
                             // it goes to the desired function

void setup()
{
  Serial.begin(9600);

  pinMode(buttonAPin, INPUT);    
  pinMode(buttonBPin, INPUT); 
  pinMode(IRLED, OUTPUT);
  pinMode(TORCH, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(debug, OUTPUT); 

//  
//  // Things from clickbutton.h
//          // pinMode(buttonB, INPUT_PULLUP);  
//      // Setup button timers (all in milliseconds / ms)
//      // (These are default if not set, but changeable 
//      // for convenience)
//  button2.debounceTime   = 20;   // Debounce timer in ms
//  // Multiclick isn't used? Just longclick. 
//  //buttonB.multiclickTime = 250;  // Time limit for multi clicks
//  button2.longClickTime  = 1000;  // time until "held-down clicks" 
//                                  //register
Serial.println("Starting void loop");
}

void loop()
{
  //Here, we check the buttons. 
  //First, we check button A: 
  Serial.println("Checking button A");
  buttonAState = digitalRead(buttonAPin);
  if (buttonAState == HIGH) {  //Button A has been pressed, what to do? 
    Serial.println("Button A is pressed");
    buttonAPushCounter++;      //Adding +1 to the counter
      Serial.println(buttonAPushCounter);
    while (buttonAState == HIGH) { delay(10);   buttonAState = digitalRead(buttonAPin);}
  }
  else { //Button A hasn't been pressed, what to do? 
         //Nothing
  }
  //Now, we check button B: 
//  Serial.println("Checking button B");
  buttonBState = digitalRead(buttonBPin);
  if (buttonBState == HIGH) {  //Button B has been pressed, what to do? 
                               //We check the counter from button A
        if (buttonAPushCounter == 0) {sleepNow();}            //Zzzz time to Sleep
        if (buttonAPushCounter == 1) {torch();}            //Torch mode
        if (buttonAPushCounter == 2) {laser();}            //Laser mode
        if (buttonAPushCounter == 3) {irmode();}  //IR mode
//        if (buttonAPushCounter == 4) {goto tvbgone;}          //TVBgone mode
//        if (buttonAPushCounter == 5) {goto onlysound;}        //Only sound
        if (buttonAPushCounter >> 5) {sleepNow();}            //Zzzz time to Sleep
  }
  else { //Button B hasn't been pressed, what to do? 
         //Nothing
  }           
  if (buttonAPushCounter != 0) { //Showing Button a push counter if it's not 0
  Serial.print("Counter:"); Serial.println(buttonAPushCounter); }
}





void torch() // Torch mode! We have two modes, toggle and not toggle. 
{            // We'll have the torch on 200ms before choosing
  Serial.println("Entering torch mode");
  delay(200);//Now we'll chose! If it's unpressed -> toggle mode
             //                 If it's pressed -> on while pressed
  buttonBState = digitalRead(buttonBPin); // We read button B
  if (buttonBState == HIGH) { //If it's pressed -> on while pressed
    // turn LED on:    
    digitalWrite(TORCH, HIGH);  
    while (buttonBState == HIGH) { delay(10);   buttonBState = digitalRead(buttonBPin);}
  }
  else {                      //If it's unpressed -> toggle mode
//[TO DO]
  }
  sleepNow();
}

void laser() //This code is the same from the torch, but changing TORCH for LASER
{            // We'll have the torch on 200ms before choosing
  Serial.println("Entering laser mode");
  delay(200);//Now we'll chose! If it's unpressed -> toggle mode
             //                 If it's pressed -> on while pressed
  buttonBState = digitalRead(buttonBPin); // We read button B
  if (buttonBState == HIGH) { //If it's pressed -> on while pressed
    // turn LED on:    
    digitalWrite(LASER, HIGH);  
    while (buttonBState == HIGH) { delay(10);   buttonBState = digitalRead(buttonBPin);}
  }
  else {                      //If it's unpressed -> toggle mode
//[TO DO]
  }
  sleepNow();
}

void irmode() 
{
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
    sleepNow();
}


void sleepNow() 
{
  Serial.println("Going to sleep");
  buttonAPushCounter = 0;
  pinMode(IRLED, LOW);
  pinMode(TORCH, LOW);
  pinMode(LASER, LOW);
  pinMode(debug, LOW); 
  
  delay(1000000);  
  
  set_sleep_mode(buttonAPin);                    // sleep mode is set here

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

