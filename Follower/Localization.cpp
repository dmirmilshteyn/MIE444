#include "Localization.h"

volatile long int leftMotorCount;
volatile long int rightMotorCount;
long int previousLeftMotorCount = 0;
long int previousRightMotorCount = 0;
int absoluteLocationX = 0;
int absoluteLocationY = 0;
double absoluteHeadingAngle = 0;
int relativeLocationX = 0;
int relativeLocationY = 0;
int relativeLocationXMeters = 0; //************ADD*************
int relativeLocationYMeters = 0; //************ADD*************
double relativeHeadingAngle = 0; //rads...right is negative, left is positive
void updateRelativeLocation();


void initializeEncoders() {
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_MOTOR), handleLeftMotorInterupt, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_MOTOR), handleRightMotorInterupt, RISING);
}

void handleLeftMotorInterupt() {
  // Note: 1 wheel rotation = GEAR_RATIO * ENCODER_TEETH_COUNT = 100.37 * 12 = 1204.44 clicks
  leftMotorCount++;
  //if(digitalRead(ENCODER_LEFT_MOTOR2)) leftMotorCount++;
  //else leftMotorCount--;

  // TODO: Handle direction
}

void handleRightMotorInterupt() {
  rightMotorCount++;
  //if(digitalRead(ENCODER_RIGHT_MOTOR2)) rightMotorCount++;
  //else leftMotorCount--;
  // TODO: Handle direction
}


void updateRelativeLocation() {
  int leftEncoderDiff = leftMotorCount - previousLeftMotorCount;
  previousLeftMotorCount = leftMotorCount;
  int rightEncoderDiff = rightMotorCount - previousRightMotorCount;
  previousRightMotorCount = rightMotorCount;

  double relativeHeadingAngleDiff;
  double travelDist;

  double leftEncoderDiffMeters = leftEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;
  double rightEncoderDiffMeters = rightEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;

  relativeHeadingAngleDiff = (rightEncoderDiffMeters - leftEncoderDiffMeters) / WHEEL_GAP;

  if (abs(relativeHeadingAngleDiff) > 0.001) {
    travelDist = ((leftEncoderDiffMeters + rightEncoderDiffMeters) * sqrt(2 - 2 * cos(relativeHeadingAngleDiff))) / (2 * relativeHeadingAngleDiff);
  }
  else {
    travelDist = ((leftEncoderDiffMeters + rightEncoderDiffMeters)  / (2));
  }

  relativeLocationXMeters += cos(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;
  relativeLocationXMeters += sin(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;
  
  relativeLocationX = round(relativeLocationXMeters / MAP_RESOLUTION);
  relativeLocationY = round(relativeLocationYMeters / MAP_RESOLUTION);

  relativeHeadingAngle += relativeHeadingAngleDiff;
}








