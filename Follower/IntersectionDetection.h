#ifndef _INTERSECTIONDETECTION_h
#define _INTERSECTIONDETECTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"
#include "Debug.h"

#define FRONT_APPROACHING_THRESHOLD 750;
#define FRONT_ON_THRESHOLD 850;
#define LEFT_APPROACHING_THRESHOLD 650;
#define LEFT_ON_THRESHOLD 850;
#define RIGHT_APPROACHING_THRESHOLD 650;
#define RIGHT_ON_THRESHOLD 850;

void ReadIntersectionSensors();
void IdentifyIntersection(int tick, int frontSensor, int leftSensor, int rightSensor, int encoderLeft, int encoderRight);

#endif

