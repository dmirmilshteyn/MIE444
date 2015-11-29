#ifndef _CHECKPOINT_h
#define _CHECKPOINT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"
#include "Core.h"
#include "Localization.h"

extern bool checkpointEventActive; //Once the checkPoint event is complete after CHECKPOINT_REACTION_DURATION time, gets set to "true"
extern long checkpointTime; //time in millis() at which a checkpoint was found. Used for  checkPointHandle() timer.
extern int ledBlinkCount; //a counter to control the blinking of the LED
extern bool ledOn; //variable stoes the state of the LED.
extern int numCheckpointsFound; //keeps track of the number of unique checkpoints the robot found
struct storedCheckpoint {int x; int y;};
extern storedCheckpoint storedCheckpoint[CHECKPOINTS_TOTAL]; //stores the relative location of all the found checkpoints

void checkPointHandle(long currentTime); //Function to handle the events that occur once checkpoint is found
void enableLED(); //turns checkpoint found indicator LED on
void disableLED(); //turns checkpoint found indicator LED off
bool checkpointFound(); //checks if the detected checkpoint has already been found before

#endif

