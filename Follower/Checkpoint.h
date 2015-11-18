#ifndef _CHECKPOINT_h
#define _CHECKPOINT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"
#include "Core.h"

extern bool checkPointFound; //Once Robot finds the checkpoint, gets set to "true"
extern bool checkPointComplete; //Once the checkPointHandle() is complete after CHECKPOINT_REACTION_DURATION time, gets set to "true"
extern int checkpointTime; //time in millis() at which the checkpoint was found. Used for  checkPointHandle() timer.
extern int ledBlinkCount; //a counter to control the blinking of the LED
extern bool ledOn; //variable stoes the state of the LED.

void checkPointHandle(int currentTime); //Function to handle the events that occur once checkpoint is found
void enableLED(); //turns checkpoint found indicator LED on
void disableLED(); //turns checkpoint found indicator LED off

#endif

