#ifndef _LOCALIZATION_h
#define _LOCALIZATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/pgmspace.h>
#include "Constants.h"
#include "MappingConstants.h"
#include "Math.h"
#include "LineFollower.h"

#define MAX_PATH_LENGTH 2
#define PATH_COUNT 5

extern volatile long int leftMotorCount;
extern volatile long int rightMotorCount;
extern long int previousLeftMotorCount;
extern long int previousRightMotorCount;
extern int absoluteLocationX;
extern int absoluteLocationY;
extern double absoluteHeadingAngle;
extern double absoluteLocationXMeters;
extern double absoluteLocationYMeters;
extern void updateRelativeLocation();
extern float pathLocation[PATH_COUNT][MAX_PATH_LENGTH + 1];

extern int currentPath;
extern int lastIntersectionMarkerId;


extern long wallEncoderTicks;
extern long wallTime;

void initializeEncoders();
void handleLeftMotorInterupt();
void handleRightMotorInterupt();

void pushDetectedIntersection(int intersectionType);
void updateIntersectionLocalization(int intersectionType);
int tryToLocalize();

int DetermineTurnDirection(double headingAngle, int currentX, int currentY, int upcomingX, int upcomingY);

#endif

