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

  double leftEncoderDiffMetres = leftEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;
  double rightEncoderDiffMetres = rightEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;

  relativeHeadingAngleDiff = (rightEncoderDiffMetres - leftEncoderDiffMetres) / WHEEL_GAP;

  if (abs(relativeHeadingAngleDiff) > 0.001) {
    travelDist = ((leftEncoderDiffMetres + rightEncoderDiffMetres) * sqrt(2 - 2 * cos(relativeHeadingAngleDiff))) / (2 * relativeHeadingAngleDiff);
  }
  else{
     travelDist = ((leftEncoderDiffMetres + rightEncoderDiffMetres)  / (2));
  }
  
  relativeLocationX += cos(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;
  relativeLocationY += sin(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;


  relativeHeadingAngle += relativeHeadingAngleDiff;
}








