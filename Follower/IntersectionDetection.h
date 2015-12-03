#ifndef _INTERSECTIONDETECTION_h
#define _INTERSECTIONDETECTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <math.h>
#include "Constants.h"
#include "Map.h"
#include "Localization.h"
#include "LineFollower.h"
#include "Planner.h"

#include "Debug.h"

#define FRONT_APPROACHING_THRESHOLD 750;
#define FRONT_ON_THRESHOLD 850;
#define LEFT_APPROACHING_THRESHOLD 650;
#define LEFT_ON_THRESHOLD 850;
#define RIGHT_APPROACHING_THRESHOLD 650;
#define RIGHT_ON_THRESHOLD 850;

#define SENSOR_LOCATION_FRONT 0
#define SENSOR_LOCATION_LEFT 1
#define SENSOR_LOCATION_RIGHT 2

extern int detectedIntersection;
extern int detectedMarkerId;

extern long lastIntersectionDetectionLeftEncoder;
extern long lastIntersectionDetectionRightEncoder;

extern int previousTestIntersection;
extern int currentTestIntersection;

extern long lastLeftEncoder;
extern long lastRightEncoder;

extern double lastTick;

inline double GetEncoderDistanceTicks() {
	return (GEAR_RATIO * ENCODER_TEETH_COUNT * (1.0 / 22));
}

void ReadIntersectionSensors(long tick);
void IdentifyIntersection(int tick, int frontSensor, int leftSensor, int rightSensor, long encoderLeft, long encoderRight);

void verifyDetectedIntersection();

int GetCurrentSensorValue(int sensorLocation);
int GetSensorOnThreshold(int sensorLocation);
int GetSensorApproachingThreshold(int sensorLocation);

bool IsSensorOn(int sensorLocation);
bool IsSensorApproaching(int sensorLocation);
bool IsSensorOnOrApproaching(int sensorLocation);

#endif

