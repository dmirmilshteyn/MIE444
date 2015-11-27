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
double relativeLocationXMeters = 0; //************ADD*************
double relativeLocationYMeters = 0; //************ADD*************
double relativeHeadingAngle = 0; //rads...right is negative, left is positive
void updateRelativeLocation();
void correctRelativeAngle();
bool correctRelativeAngleDone = false;

void initializeEncoders() {
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_MOTOR), handleLeftMotorInterupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_MOTOR), handleRightMotorInterupt, CHANGE);

  pinMode(ENCODER_LEFT_MOTORB, INPUT);
  pinMode(ENCODER_RIGHT_MOTORB, INPUT);
}

void handleLeftMotorInterupt() {
  // Note: 1 wheel rotation = GEAR_RATIO * ENCODER_TEETH_COUNT = 100.37 * 12 = 1204.44 clicks
  if (leftForward == true) {
    leftMotorCount++;
  }
  else {
    leftMotorCount--;
  }
  /*if(digitalReadFast(ENCODER_LEFT_MOTORB)) leftMotorCount++;
    else leftMotorCount--;*/

  // TODO: Handle direction
}

void handleRightMotorInterupt() {
  if (rightForward == true) {
    rightMotorCount++;
  }
  else {
    rightMotorCount--;
  }
  /*if(digitalReadFast(ENCODER_RIGHT_MOTORB)) rightMotorCount++;
    else leftMotorCount--;*/
  // TODO: Handle direction
}

//if robot is placed at an angle with the line, this code will correct the angle and position after the robot has travelled 0.4m
void correctRelativeAngle() {
  double travelDistFromStart = sqrt(pow(relativeLocationXMeters, 2) + pow(relativeLocationYMeters, 2));
  double relativeCorrectionAngle;
  if (travelDistFromStart > 0.4) {
    relativeCorrectionAngle = atan(relativeLocationYMeters / relativeLocationXMeters);
    relativeHeadingAngle -= relativeCorrectionAngle;
    relativeLocationXMeters = travelDistFromStart;
    relativeLocationYMeters = 0;
    relativeLocationX = round(relativeLocationXMeters / MAP_RESOLUTION);
    relativeLocationY = round(relativeLocationYMeters / MAP_RESOLUTION);
    correctRelativeAngleDone = true;
  }
}
void updateRelativeLocation() {
  long int leftEncoderDiff = leftMotorCount - previousLeftMotorCount;
  previousLeftMotorCount = leftEncoderDiff + previousLeftMotorCount;
  long int rightEncoderDiff = rightMotorCount - previousRightMotorCount;
  previousRightMotorCount = rightEncoderDiff + previousRightMotorCount;

  double relativeHeadingAngleDiff;
  double travelDist;

  double leftEncoderDiffMeters = leftEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;
  double rightEncoderDiffMeters = rightEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;



  relativeHeadingAngleDiff = (rightEncoderDiffMeters - leftEncoderDiffMeters) / WHEEL_GAP;

  //Serial.print(relativeHeadingAngleDiff);

  if (abs(relativeHeadingAngleDiff) > 0.001) {
    travelDist = abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) * sqrt(2 - 2 * cos(relativeHeadingAngleDiff))) / (2 * relativeHeadingAngleDiff));
  }
  else {
    travelDist = abs(((leftEncoderDiffMeters + rightEncoderDiffMeters)  / (2)));
  }

  relativeLocationXMeters += cos(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;
  relativeLocationYMeters += sin(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;

  relativeLocationX = round(relativeLocationXMeters / MAP_RESOLUTION);
  relativeLocationY = round(relativeLocationYMeters / MAP_RESOLUTION);

  relativeHeadingAngle += relativeHeadingAngleDiff;

  //Serial.print(" ");
  //Serial.println(relativeHeadingAngle);
  //  if (correctRelativeAngleDone == false) {
  //    correctRelativeAngle();
  //  }
}








