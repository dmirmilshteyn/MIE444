#include "LineFollower.h"

// Global variables for line following

int followerState = FOLLOWER_STATE_ONLINE;
int turnState = TURN_STATE_DEFAULT;

float lastError;
long integral = 0;
bool leftForward = true;
bool rightForward = true;
int wallSensorBuffer = 0;
int wallBufferCounter = 0;
double wallDistance = 0;

int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
int stallPWM;//PWM at which the motor stalls

unsigned long previousTime;

unsigned long wallIdentificationStartTime = 0;

int wallSampleCount = 0;
int wallSampleTotal = 0;

float readLeft;
float readRight;

int determineStallPWMDone = 0;

bool isRealigning = false;
long lastRealignLeftMotorCount = 0;
long lastRealignRightMotorCount = 0;

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
  if (Ki * abs(integral) > 130) {
    if(integral>0){
      integral = 100/Ki;
    }
    else{
      integral = -100/Ki;
    }
  }
  derivative = (currentError - lastError) / timeDifference;
  lastError = currentError;
  controller = Kp * currentError + Ki * integral + Kd * derivative;

  //Serial.print(" error: ");
  //Serial.print(currentError);
  //Serial.print(" controller: ");
  //Serial.println(controller);
  MotorSpeeds motorSpeeds = driveMotorsPID(controller, derivative);
  publishLaneFollowingData(currentTime, motorSpeeds, currentError, integral, derivative, controller, readLeft, readRight);

  previousTime = currentTime;
}

MotorSpeeds driveMotorsBasic(float controller, float adjustedSpeed, float speedOffset) {
  MotorSpeeds newMotorSpeeds;

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

void updateFollowerState(unsigned long currentTime) {
  if (readLeft < 600 && readRight < 600 && followerState != FOLLOWER_STATE_RIGHT && followerState != FOLLOWER_STATE_LEFT) {
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

  if (followerState == FOLLOWER_STATE_IDENTIFY_WALL) {
    if (currentTime > wallIdentificationStartTime + 1000) {
      bool isBlack = ((double)wallSampleCount / (double)wallSampleTotal) > 0.5;
      if (!isBlack) {
        // TODO: Debugging for now
        numCheckpointsFound = CHECKPOINTS_TOTAL;
        // Detected a white wall -> this is the starting location
        //if (pgm_read_byte(&(intersections[lastIntersectionMarkerId].id)) == pgm_read_byte(&(intersections[(int)pathLocation[currentPath][0]].id))) {
        if (numCheckpointsFound < CHECKPOINTS_TOTAL) {
          followerState = FOLLOWER_STATE_WALL_START_GOBACK;
        }
        else {
          followerState = FOLLOWER_STATE_WALL_START_DONE;
        }
      }
      else {
        // Detected a black wall - handle the deadend state
        followerState = FOLLOWER_STATE_WALL_DEADEND;
      }

      wallSampleCount = 0;
      wallSampleTotal = 0;
    }
  }
}

MotorSpeeds driveMotorsPID(float controller, float derivative) {
  //should make avg speed inversely proportional to the controller...will slow down if error is high
  float speedOffsetFactor = abs(controller / 255); //(-exp(-abs(controller) / 120) + 1) * 1.15;
  if (speedOffsetFactor > 1)speedOffsetFactor = 1;
  float adjustedSpeed = averageMotorSpeed;// - DERIVATIVE_SPEED_ADJUST * derivative * (averageMotorSpeed - (stallPWM)) / (255 - stallPWM);
  //float adjustedSpeed = averageMotorSpeed;
  float speedOffset = speedOffsetFactor * (adjustedSpeed); //abs((controller * (adjustedSpeed - (stallPWM)) / (255 - stallPWM))); //controller offset is scaled with average speed (255-stallPWM). Cutoff at stallPWM.

  MotorSpeeds motorSpeeds;
  if (followerState == FOLLOWER_STATE_ONLINE) {
    motorSpeeds = driveMotorsBasic(controller, adjustedSpeed, speedOffset);
  }
  else if (followerState == FOLLOWER_STATE_OFFLINE) {
    motorSpeeds.left = -(adjustedSpeed * 1.2);
    motorSpeeds.right = adjustedSpeed * 1;


    //    isRealigning = true;
    //    lastRealignLeftMotorCount = -1;
    //    lastRealignRightMotorCount = -1;
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
        motorSpeeds.left = -averageMotorSpeed * 0.6;
        motorSpeeds.right = averageMotorSpeed * 1.3;
      }
      else if (followerState == FOLLOWER_STATE_RIGHT) {
        // Turn right
        motorSpeeds.right = -averageMotorSpeed * 0.6;
        motorSpeeds.left = averageMotorSpeed * 1.3;
      }
      else if (followerState == FOLLOWER_STATE_WALL_DEADEND) {
        // Turn right

        isRealigning = true;
        lastRealignLeftMotorCount = -1;
        lastRealignRightMotorCount = -1;
        motorSpeeds.right = -(adjustedSpeed * 1.2);
        motorSpeeds.left = adjustedSpeed * 1;


      }
    }
  }
  else if (followerState == FOLLOWER_STATE_IDENTIFY_WALL) {
    // While checking, stop the motors
    motorSpeeds.right = 0;
    motorSpeeds.left = 0;

    wallSampleCount += digitalRead(WALL_COLOUR_SENSOR);
    wallSampleTotal++;
  }
  else if (followerState == FOLLOWER_STATE_WALL_START_DONE) {
    // Aaaaannnnnndd we're done!

    motorSpeeds.right = 0;
    motorSpeeds.left = 0;
  }

  //next 4 if statements drive the left and right motors forward or back depending on the signs of newLeftMotorSpeed and newRightMotorSpeed
#ifndef NOMOTORS
  if (motorSpeeds.left >= 0) {
    leftForward = true;
    analogWrite(BIN2_LEFT_MOTOR, 0);
    analogWrite(BIN1_LEFT_MOTOR, motorSpeeds.left);//drives left motor forward
  }
  else {
    leftForward = false;
    analogWrite(BIN1_LEFT_MOTOR, 0);
    analogWrite(BIN2_LEFT_MOTOR, -motorSpeeds.left);//drives left motor reverse
  }

  if (motorSpeeds.right >= 0) {
    rightForward = true;
    analogWrite(AIN2_RIGHT_MOTOR, 0);
    analogWrite(AIN1_RIGHT_MOTOR, motorSpeeds.right);//drives right motor forward
  }
  else {
    rightForward = false;
    analogWrite(AIN1_RIGHT_MOTOR, 0);
    analogWrite(AIN2_RIGHT_MOTOR, -motorSpeeds.right);//drives right motor reverse
  }
#endif

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


void wallDetection(unsigned long currentTime) {

  wallSensorBuffer += analogRead(WALL_DISTANCE_SENSOR);
  wallBufferCounter++;
  if (wallBufferCounter >= 10) {
    wallDistance = ((double)wallSensorBuffer) / wallBufferCounter;
    wallBufferCounter = 0;
    wallSensorBuffer = 0;
  }

  //	Serial.print("dist: ");
  //	Serial.print(wallDistance);

  if (followerState == FOLLOWER_STATE_ONLINE) {

    if (wallDistance > 190 ) {
      wallDistance = 0;

      followerState = FOLLOWER_STATE_IDENTIFY_WALL;
      wallIdentificationStartTime = currentTime;
    }

  }
}







