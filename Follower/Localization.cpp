#include "Localization.h"

volatile int leftMotorCount;
volatile int rightMotorCount;

void initializeEncoders() {
	attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_MOTOR), handleLeftMotorInterupt, RISING);
	attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_MOTOR), handleRightMotorInterupt, RISING);
}

void handleLeftMotorInterupt() {
	// Note: 1 wheel rotation = GEAR_RATIO * ENCODER_TEETH_COUNT = 100.37 * 3 = 301.11 clicks
	leftMotorCount++;

	// TODO: Handle direction
}

void handleRightMotorInterupt() {
	rightMotorCount++;

	// TODO: Handle direction
}