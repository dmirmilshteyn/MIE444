#ifndef _LOCALIZATION_h
#define _LOCALIZATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Constants.h"
#include "MappingConstants.h"
#include "Mapping.h"
#include "Math.h"

extern volatile long int leftMotorCount;
extern volatile long int rightMotorCount;
extern long int previousLeftMotorCount;
extern long int previousRightMotorCount;
extern int absoluteLocationX;
extern int absoluteLocationY;
extern double absoluteHeadingAngle;
extern int relativeLocationX;
extern int relativeLocationY;
extern int relativeLocationXMeters; //************ADD*************
extern int relativeLocationYMeters; //************ADD*************
extern double relativeHeadingAngle;
extern void updateRelativeLocation();
extern void correctRelativeAngle();
extern bool correctRelativeAngleDone;

void initializeEncoders();
void handleLeftMotorInterupt();
void handleRightMotorInterupt();

#endif

