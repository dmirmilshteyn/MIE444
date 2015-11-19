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

extern volatile int leftMotorCount;
extern volatile int rightMotorCount;

void initializeEncoders();
void handleLeftMotorInterupt();
void handleRightMotorInterupt();

#endif

