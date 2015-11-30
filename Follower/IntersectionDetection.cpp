#include "IntersectionDetection.h"

int detectedIntersection = INTERSECTION_TYPE_NONE;

int currentFront = 0;
int currentLeft = 0;
int currentRight = 0;

int previousTestIntersection = INTERSECTION_TYPE_NONE;
int currentTestIntersection = INTERSECTION_TYPE_NONE;

long lastLeftEncoder = 0;
long lastRightEncoder = 0;

long lastIntersectionDetectionLeftEncoder = 0;
long lastIntersectionDetectionRightEncoder = 0;

double lastTick = 0;

void ProcessDetectedIntersection(int detectedIntersectionType) {
	if (currentPath == -1) {
		pushDetectedIntersection(detectedIntersectionType);
	} else if (currentPath > -1) {
		updateIntersectionLocalization(detectedIntersectionType);
	}

	switch (detectedIntersectionType) {
	case INTERSECTION_TYPE_LEFTTURN:
		followerState = FOLLOWER_STATE_LEFT;
		break;
	case INTERSECTION_TYPE_RIGHTTURN:
		followerState = FOLLOWER_STATE_RIGHT;
		break;
	case INTERSECTION_TYPE_T:
		followerState = FOLLOWER_STATE_RIGHT;
		break;
	case INTERSECTION_TYPE_TRIGHT:
		followerState = FOLLOWER_STATE_RIGHT;
		break;
	case INTERSECTION_TYPE_TLEFT:
		followerState = FOLLOWER_STATE_LEFT;
		break;
	}

#ifdef NOMOTORS
	followerState == FOLLOWER_STATE_ONLINE;
	detectedIntersection = INTERSECTION_TYPE_NONE;

	lastIntersectionDetectionLeftEncoder = leftMotorCount;
	lastIntersectionDetectionRightEncoder = rightMotorCount;
#endif
}

void ReadIntersectionSensors(long tick) {
	int sensorA = analogRead(LINE_SENSOR_1);
	int sensorB = analogRead(LINE_SENSOR_2);
	int sensorC = analogRead(LINE_SENSOR_3);

	publishIntersectionDetectionData(tick, sensorA, sensorB, sensorC, detectedIntersection);

  currentFront = sensorA;
  currentLeft = sensorB;
  currentRight = sensorC;
  
	if (followerState == FOLLOWER_STATE_ONLINE) {
		if (leftMotorCount > lastIntersectionDetectionLeftEncoder + 600 / 4 &&
			rightMotorCount > lastIntersectionDetectionRightEncoder + 600 / 4) {
			IdentifyIntersection(tick, sensorA, sensorB, sensorC, leftMotorCount, rightMotorCount);

			if (detectedIntersection != INTERSECTION_TYPE_NONE) {
				ProcessDetectedIntersection(detectedIntersection);
			}
		}
	}
}

void IdentifyIntersection(int tick, int frontSensor, int leftSensor, int rightSensor, int encoderLeft, int encoderRight) {
	currentTestIntersection = INTERSECTION_TYPE_NONE;

	if (IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT)) {
		currentTestIntersection = INTERSECTION_TYPE_LEFTTURN;
	}
	else if (IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
		currentTestIntersection = INTERSECTION_TYPE_RIGHTTURN;
	}

	if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) && IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT)) {
		currentTestIntersection = INTERSECTION_TYPE_TLEFT;
	}
	else if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) && IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
		currentTestIntersection = INTERSECTION_TYPE_TRIGHT;
	}

	if (IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT) && IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
		currentTestIntersection = INTERSECTION_TYPE_T;
	}

	if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) && IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT) && IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
		currentTestIntersection = INTERSECTION_TYPE_CROSS;
	}

	if (abs(encoderLeft - lastLeftEncoder) > 75) {
		detectedIntersection = currentTestIntersection;
	}
	else {
		double currentTick = (encoderLeft + encoderRight) / 2;
		if (currentTestIntersection != previousTestIntersection) {
			detectedIntersection = INTERSECTION_TYPE_NONE;

			previousTestIntersection = currentTestIntersection;
			lastTick = currentTick;
		}
		else {
			double testTick = lastTick + GetEncoderDistanceTicks();
			if (currentTick > testTick) {
				detectedIntersection = currentTestIntersection;
				lastTick = currentTick;
			}
		}
	}

	lastLeftEncoder = encoderLeft;
	lastRightEncoder = encoderRight;
}

int GetCurrentSensorValue(int sensorLocation) {
	switch (sensorLocation) {
	case SENSOR_LOCATION_FRONT:
		return currentFront;
	case SENSOR_LOCATION_LEFT:
		return currentLeft;
	case SENSOR_LOCATION_RIGHT:
		return currentRight;
	}
}

int GetSensorOnThreshold(int sensorLocation) {
	switch (sensorLocation) {
	case SENSOR_LOCATION_FRONT:
		return FRONT_ON_THRESHOLD;
	case SENSOR_LOCATION_LEFT:
		return LEFT_ON_THRESHOLD;
	case SENSOR_LOCATION_RIGHT:
		return RIGHT_ON_THRESHOLD;
	}
}

int GetSensorApproachingThreshold(int sensorLocation) {
	switch (sensorLocation) {
	case SENSOR_LOCATION_FRONT:
		return FRONT_APPROACHING_THRESHOLD;
	case SENSOR_LOCATION_LEFT:
		return LEFT_APPROACHING_THRESHOLD;
	case SENSOR_LOCATION_RIGHT:
		return RIGHT_APPROACHING_THRESHOLD;
	}
}

bool IsSensorOn(int sensorLocation) {
	int onThreshold = GetSensorOnThreshold(sensorLocation);
	int value = GetCurrentSensorValue(sensorLocation);

	return (value >= onThreshold);
}

bool IsSensorApproaching(int sensorLocation) {
	int approachingThreshold = GetSensorApproachingThreshold(sensorLocation);
	int onThreshold = GetSensorOnThreshold(sensorLocation);
	int value = GetCurrentSensorValue(sensorLocation);

	return (value > approachingThreshold && value < onThreshold);
}

bool IsSensorOnOrApproaching(int sensorLocation) {
	return (IsSensorApproaching(sensorLocation) || IsSensorOn(sensorLocation));
}
