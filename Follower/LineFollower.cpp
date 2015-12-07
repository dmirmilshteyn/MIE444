#include "LineFollower.h"

// Global variables for line following

int followerState = FOLLOWER_STATE_ONLINE;
int turnState = TURN_STATE_DEFAULT;

/*************LIne following PID constants***********/
float Kp = 2.1;
float Ki = 0.003;
float Kd = 700;
float lastError;
long integral = 0;
bool leftForward = true;
bool rightForward = true;

/*************Wall Detection constants***********/
int wallSensorBuffer = 0;
int wallBufferCounter = 0;
double wallDistance = 0;

int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
int stallPWM;//PWM at which the motor stalls

unsigned long previousTime;

unsigned long wallIdentificationStartTime = 0;

int wallSampleCount = 0;
int wallSampleTotal = 0;

long wallEncoderTicks = 0;
long wallTime = 0;

float readLeft;
float readRight;

int determineStallPWMDone = 0;

bool isRealigning = false;
long lastRealignLeftMotorCount = 0;
long lastRealignRightMotorCount = 0;

int currentLeftMotorSpeed = 0;
int currentRightMotorSpeed = 0;

//CALCULATED THE PID CONTROLLER PARAMETERS
void followLaneAnalog(unsigned long currentTime) {
  determineStallPWM();
  float timeDifference = currentTime - previousTime;
  if (timeDifference < 1) {
    timeDifference = 1;
  }
  float derivative;
  float currentError = getLaneError(); //robot too far left is negative...too far right is positive
  float controller;


  integral = integral + (((currentError + lastError) / 2) * timeDifference);
  if (Ki * abs(integral) > 150) {
    if (integral > 0) {
      integral = 150 / Ki;
    }
    else {
      integral = -150 / Ki;
    }
  }
  derivative = (currentError - lastError) / timeDifference;
  lastError = currentError;
  controller = Kp * currentError + Ki * integral + Kd * derivative;

  MotorSpeeds motorSpeeds = driveMotorsPID(controller, derivative);
  publishLaneFollowingData(currentTime, motorSpeeds, currentError, integral, derivative, controller, readLeft, readRight);

  previousTime = currentTime;
}

//DRIVES MOTORS BASED ON PID CONTROLLER WHEN FOLLOWER_STATE_ONLINE
MotorSpeeds driveMotorsBasic(float controller, float adjustedSpeed) {
  MotorSpeeds newMotorSpeeds;

  float speedOffsetFactor = abs(controller / 255);
  if (speedOffsetFactor > 1)speedOffsetFactor = 1;

  float speedOffset = speedOffsetFactor * (adjustedSpeed);

  /*int newLeftMotorSpeed;
    int newRightMotorSpeed;*/

  if (controller <= 0) {
    newMotorSpeeds.left = adjustedSpeed;
    newMotorSpeeds.right = adjustedSpeed - speedOffset;
  }
  else if (controller > 0) {
    newMotorSpeeds.left = adjustedSpeed - speedOffset;
    newMotorSpeeds.right = adjustedSpeed;
  }

  //Controls what to do if the adjustedSpeed is too low
  //Function will assume the car is stopped and will try to keep the car centered with the line
  if (adjustedSpeed < stallPWM) {
    if (abs(lastError) > 50) {
      newMotorSpeeds.left = -signOf(controller) * (stallPWM + 10);
      newMotorSpeeds.right = signOf(controller) * (stallPWM + 10);
    }
    else {
      newMotorSpeeds.left = 0;
      newMotorSpeeds.right = 0;
    }

  }
  //checks if any of the motors are below stall PWM and sets them to zero
  else if (newMotorSpeeds.left < stallPWM || newMotorSpeeds.right < stallPWM) {
    if (abs(newMotorSpeeds.left) < stallPWM) {
      newMotorSpeeds.left = 0;
    }
    else if (abs(newMotorSpeeds.right) < stallPWM) {
      newMotorSpeeds.right = 0;
    }
  }

  /*if (readLeft < 750 && readRight < 750) {
    newRightMotorSpeed = 0;

    newLeftMotorSpeed = 0;

    }*/
  //stops the car if it left the line (on white)

  return newMotorSpeeds;
}

//UPDATED THE FOLLOWER STATE
void updateFollowerState(unsigned long currentTime) {
  if (readLeft < 600 && readRight < 600 &&
      followerState != FOLLOWER_STATE_RIGHT &&
      followerState != FOLLOWER_STATE_LEFT &&
      followerState != FOLLOWER_STATE_STRAIGHT &&
      followerState != FOLLOWER_STATE_WALL_DEADEND) {
    followerState = FOLLOWER_STATE_OFFLINE;
  }
  else if (followerState == FOLLOWER_STATE_OFFLINE) {
    followerState = FOLLOWER_STATE_ONLINE;
  }

  if (isRealigning && readLeft >= 600 && readRight >= 600) {
    if (lastRealignLeftMotorCount == -1 && lastRealignRightMotorCount == -1) {
      lastRealignLeftMotorCount = leftMotorCount;
      lastRealignRightMotorCount = rightMotorCount;
    } else if (leftMotorCount > lastRealignLeftMotorCount + 200 && rightMotorCount > lastRealignRightMotorCount + 200) {
      detectedIntersection = INTERSECTION_TYPE_NONE;
      isRealigning = false;
    }
  }
  if (followerState == FOLLOWER_STATE_STRAIGHT && detectedIntersection == INTERSECTION_TYPE_NONE && !IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT) && !IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT) && readLeft >= 600 && readRight >= 600) {
    followerState = FOLLOWER_STATE_ONLINE;
  }
}

//THIS FUNCTION DRIVES THE MOTORS BASED ON THE STATE OF THE FOLLOWER.
//driveMotorsBasic IS ENVOKED IF THE FOLLOWER IS IN ITS DEFAULT LINE FOLLOWING STATE
MotorSpeeds driveMotorsPID(float controller, float derivative) {

  float adjustedSpeed = averageMotorSpeed;
  MotorSpeeds motorSpeeds;
  if (followerState == FOLLOWER_STATE_ONLINE || followerState == FOLLOWER_STATE_STRAIGHT) {
    motorSpeeds = driveMotorsBasic(controller, adjustedSpeed);
  }
  else if (followerState == FOLLOWER_STATE_OFFLINE) {
    motorSpeeds.left = -(adjustedSpeed * 1.2);
    motorSpeeds.right = adjustedSpeed * 1;
  }
  else if (followerState == FOLLOWER_STATE_WALL_START_DONE) {
    motorSpeeds.right = 0;
    motorSpeeds.left = 0;
  }

  else if (followerState == FOLLOWER_STATE_LEFT || followerState == FOLLOWER_STATE_RIGHT || followerState == FOLLOWER_STATE_WALL_DEADEND) {
    switch (turnState) {
      case TURN_STATE_DEFAULT:
        if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) == false) {
          turnState = TURN_STATE_HIT_WHITE;
        }
        break;
      case TURN_STATE_HIT_WHITE:
        if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT)) {
          turnState = TURN_STATE_HIT_BLACK;
        }
        break;
    }

    if (turnState == TURN_STATE_HIT_BLACK) {
      turnState = TURN_STATE_DEFAULT;
      followerState = FOLLOWER_STATE_ONLINE;

      motorSpeeds.left = 0;
      motorSpeeds.right = 0;
    }
    else {
      if (followerState == FOLLOWER_STATE_LEFT) {
        // Turn left
        motorSpeeds.left = -averageMotorSpeed * 1.2;
        motorSpeeds.right = averageMotorSpeed * 1.25;
      }
      else if (followerState == FOLLOWER_STATE_RIGHT) {
        // Turn right
        motorSpeeds.right = -averageMotorSpeed * 1.2;
        motorSpeeds.left = averageMotorSpeed * 1.25;
      }
      else if (followerState == FOLLOWER_STATE_WALL_DEADEND) {
        // Turn right
        wallTime = millis();
        isRealigning = true;
        lastRealignLeftMotorCount = -1;
        lastRealignRightMotorCount = -1;
        motorSpeeds.right = -(adjustedSpeed * 1.2);
        motorSpeeds.left = adjustedSpeed * 1;

        // Allow the planner to handle any cases before finalizing the rotation
        motorSpeeds = ProcessBeforeDeadEnd(motorSpeeds);
      }
    }
  }

  else if (followerState == FOLLOWER_STATE_WALL_START_DONE) {
    // Aaaaannnnnndd we're done!

    motorSpeeds.right = 0;
    motorSpeeds.left = 0;
  }

  driveMotorsAtSpeed(motorSpeeds);

  if (motorSpeeds.left == 0 && motorSpeeds.right == 0) {
    //delay(2000);
    lastIntersectionDetectionLeftEncoder = leftMotorCount;
    lastIntersectionDetectionRightEncoder = rightMotorCount;
  }

  return motorSpeeds;
}

//Function returns error of car's position relative to the lane
float getLaneError() {
  readLeft = analogRead(LINE_FOLLOW_SENSOR_LEFT);
  readRight = analogRead(LINE_FOLLOW_SENSOR_RIGHT);

  return readLeft - readRight - 32;
}

//THIS FUNCTION WILL DETERMINE THE PWM AT WHICH THE MOTORS STALL
//BY INCREMENTING PWM UNTIL THE ROVER STARTS MOVING.
//IT IS TO ENSURE THE MOTORS NEVER OPERATE AT STALL PWM TO PREVENT DAMAGE.
void determineStallPWM() {
  if (determineStallPWMDone == 0) {
    int i = 0;
    analogWrite(BIN2_LEFT_MOTOR, 0);
    analogWrite(AIN2_RIGHT_MOTOR, 0);
    do {
      delay(5);
      analogWrite(BIN1_LEFT_MOTOR, i);//drives left motor forward
      analogWrite(AIN1_RIGHT_MOTOR, i);//drives right motor forward
      i++;
    } while (leftMotorCount < 10 || rightMotorCount < 10); //((previousLeftMotorCount - leftMotorCount) / (1 / 1000) < (1204 * 0.05));
    stallPWM = i;//i;// * 1.2;
    averageMotorSpeed = 90;//(255 - stallPWM) * 0.2 + stallPWM;
    leftMotorCount = 0;
    rightMotorCount = 0;
    determineStallPWMDone = 1;

    //publishSyncData();
  }
}

//CODE FOR WALL DETECTION
//FOLLOWER STATE WILL CHANGE TO FOLLOWER_STATE_WALL_DEADEND ONCE A WALL IS DETECTED
void wallDetection(unsigned long currentTime) {

  wallSensorBuffer += analogRead(WALL_DISTANCE_SENSOR);
  wallBufferCounter++;
  if (wallBufferCounter >= 10) {
    wallDistance = ((double)wallSensorBuffer) / wallBufferCounter;
    wallBufferCounter = 0;
    wallSensorBuffer = 0;
  }
  if (followerState == FOLLOWER_STATE_ONLINE) {

    if (wallDistance > 200 ) {
      wallDistance = 0;
      followerState = FOLLOWER_STATE_WALL_DEADEND;
    }

  }
}

void wallDetection2(unsigned long currentTime) {

  if (followerState == FOLLOWER_STATE_ONLINE) {
    if (currentTime > wallTime + 1000) {
      wallTime = currentTime;
      if ((leftMotorCount + rightMotorCount) / 2 < wallEncoderTicks + 10) {
        followerState = FOLLOWER_STATE_WALL_DEADEND;
      }
      wallTime = currentTime;
      wallEncoderTicks = (leftMotorCount + rightMotorCount) / 2;
    }
  }
}

void driveMotorsAtSpeed(MotorSpeeds speeds) {
#ifndef NOMOTORS
  //next 4 if statements drive the left and right motors forward or back depending on the signs of newLeftMotorSpeed and newRightMotorSpeed
  if (speeds.left >= 0) {
    leftForward = true;
    analogWrite(BIN2_LEFT_MOTOR, 0);
    analogWrite(BIN1_LEFT_MOTOR, speeds.left);//drives left motor forward
  }
  else {
    leftForward = false;
    analogWrite(BIN1_LEFT_MOTOR, 0);
    analogWrite(BIN2_LEFT_MOTOR, -speeds.left);//drives left motor reverse
  }

  if (speeds.right >= 0) {
    rightForward = true;
    analogWrite(AIN2_RIGHT_MOTOR, 0);
    analogWrite(AIN1_RIGHT_MOTOR, speeds.right);//drives right motor forward
  }
  else {
    rightForward = false;
    analogWrite(AIN1_RIGHT_MOTOR, 0);
    analogWrite(AIN2_RIGHT_MOTOR, -speeds.right);//drives right motor reverse
  }

  currentLeftMotorSpeed = speeds.left;
  currentRightMotorSpeed = speeds.right;
#endif
}





