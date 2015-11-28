#include "LineFollower.h"

// Global variables for line following

float lastError;
float integral;
bool leftForward = true;
bool rightForward = true;

int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
int stallPWM;//PWM at which the motor stalls

int previousTime;

float readLeft;
float readRight;

int determineStallPWMDone = 0;

void followLaneAnalog(int currentTime) {
  determineStallPWM();
  float timeDifference = currentTime - previousTime;
  if (timeDifference < 1) {
    timeDifference = 1;
  }
  float derivative;
  float currentError = getLaneError(); //robot too far left is negative...too far right is positive
  float controller;


  integral = integral + (((currentError + lastError) / 2) * timeDifference);
  derivative = (currentError - lastError) / timeDifference;
  lastError = currentError;
  controller = Kp * currentError + Ki * integral + Kd * derivative;

  MotorSpeeds motorSpeeds = driveMotorsPID(controller, derivative);

  publishLaneFollowingData(motorSpeeds, currentError, integral, derivative, controller, readLeft, readRight);

  previousTime = currentTime;
}

MotorSpeeds driveMotorsPID(float controller, float derivative) {

  //should make avg speed inversely proportional to the controller...will slow down if error is high
  float speedOffsetFactor = -exp(-abs(controller) / 120) + 1;
  float adjustedSpeed = averageMotorSpeed;// - DERIVATIVE_SPEED_ADJUST * derivative * (averageMotorSpeed - (stallPWM)) / (255 - stallPWM);
  //float adjustedSpeed = averageMotorSpeed;
  float speedOffset = speedOffsetFactor * (adjustedSpeed - stallPWM); //abs((controller * (adjustedSpeed - (stallPWM)) / (255 - stallPWM))); //controller offset is scaled with average speed (255-stallPWM). Cutoff at stallPWM.
  MotorSpeeds newMotorSpeeds;

  Serial.println(speedOffset);

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
  if (readLeft < 600 && readRight < 600) {
    newMotorSpeeds.right = -(adjustedSpeed*1.22);//*1.22

    newMotorSpeeds.left = adjustedSpeed*0.8;//*0.8

  }

  //next 4 if statements drive the left and right motors forward or back depending on the signs of newLeftMotorSpeed and newRightMotorSpeed
  if (newMotorSpeeds.left >= 0) {
    leftForward = true;
    analogWrite(BIN2_LEFT_MOTOR, 0);
    analogWrite(BIN1_LEFT_MOTOR, newMotorSpeeds.left);//drives left motor forward
  }
  else {
    leftForward = false;
    analogWrite(BIN1_LEFT_MOTOR, 0);
    analogWrite(BIN2_LEFT_MOTOR, -newMotorSpeeds.left);//drives left motor reverse
  }

  if (newMotorSpeeds.right >= 0) {
    rightForward = true;
    analogWrite(AIN2_RIGHT_MOTOR, 0);
    analogWrite(AIN1_RIGHT_MOTOR, newMotorSpeeds.right);//drives right motor forward
  }
  else {
    rightForward = false;
    analogWrite(AIN1_RIGHT_MOTOR, 0);
    analogWrite(AIN2_RIGHT_MOTOR, -newMotorSpeeds.right);//drives right motor reverse
  }

  return newMotorSpeeds;
}

//Function returns error of car's position relative to the lane
float getLaneError() {
  readLeft = analogRead(LINE_FOLLOW_SENSOR_LEFT);
  readRight = analogRead(LINE_FOLLOW_SENSOR_RIGHT);

  return readLeft - readRight;
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
    stallPWM = 0;//i;// * 1.2;
    averageMotorSpeed = 85;//(255 - stallPWM) * 0.2 + stallPWM;
    leftMotorCount = 0;
    rightMotorCount = 0;
    determineStallPWMDone = 1;

    //publishSyncData();
  }
}
