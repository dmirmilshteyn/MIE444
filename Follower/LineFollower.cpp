#include "LineFollower.h"

// Global variables for line following

int followerState = FOLLOWER_STATE_ONLINE;
int turnState = TURN_STATE_DEFAULT;

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

void followLaneAnalog(long currentTime) {
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

void updateFollowerState() {
	if (readLeft < 600 && readRight < 600) {
		followerState = FOLLOWER_STATE_OFFLINE;
	}
	else if (followerState == FOLLOWER_STATE_OFFLINE || followerState == FOLLOWER_STATE_REALIGN) {
		followerState = FOLLOWER_STATE_ONLINE;
	}
}

MotorSpeeds driveMotorsPID(float controller, float derivative) {
  //should make avg speed inversely proportional to the controller...will slow down if error is high
  float speedOffsetFactor = -exp(-abs(controller) / 120) + 1;
  float adjustedSpeed = averageMotorSpeed;// - DERIVATIVE_SPEED_ADJUST * derivative * (averageMotorSpeed - (stallPWM)) / (255 - stallPWM);
  //float adjustedSpeed = averageMotorSpeed;
  float speedOffset = speedOffsetFactor * (adjustedSpeed - stallPWM); //abs((controller * (adjustedSpeed - (stallPWM)) / (255 - stallPWM))); //controller offset is scaled with average speed (255-stallPWM). Cutoff at stallPWM.
  
  MotorSpeeds motorSpeeds;
  if (followerState == FOLLOWER_STATE_ONLINE) {
	  motorSpeeds = driveMotorsBasic(controller, adjustedSpeed, speedOffset);
  }
  else if (followerState == FOLLOWER_STATE_OFFLINE) {
	  motorSpeeds.right = -(adjustedSpeed*1.22);
	  motorSpeeds.left = adjustedSpeed*0.8;
  }
  else if (followerState == FOLLOWER_STATE_LEFT || followerState == FOLLOWER_STATE_RIGHT) {
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
		  followerState = FOLLOWER_STATE_REALIGN;

		  motorSpeeds.left = 0;
		  motorSpeeds.right = 0;
	  }
	  else {
		  if (followerState == FOLLOWER_STATE_LEFT) {
			  // Turn left
			  motorSpeeds.left = -averageMotorSpeed*1.22;
			  motorSpeeds.right = averageMotorSpeed*1;
		  }
		  else if (followerState == FOLLOWER_STATE_RIGHT) {
			  // Turn right
			  motorSpeeds.right = -averageMotorSpeed*1.22;
			  motorSpeeds.left = averageMotorSpeed*1;
		  }
	  }
  }

  //next 4 if statements drive the left and right motors forward or back depending on the signs of newLeftMotorSpeed and newRightMotorSpeed
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

  if (motorSpeeds.left == 0 && motorSpeeds.right == 0) {
      //delay(500);

	  lastIntersectionDetectionLeftEncoder = leftMotorCount;
	  lastIntersectionDetectionRightEncoder = rightMotorCount;
  }

  return motorSpeeds;
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
    /*do {
      delay(5);
      analogWrite(BIN1_LEFT_MOTOR, i);//drives left motor forward
      analogWrite(AIN1_RIGHT_MOTOR, i);//drives right motor forward
      i++;
    } while (leftMotorCount < 10 || rightMotorCount < 10); //((previousLeftMotorCount - leftMotorCount) / (1 / 1000) < (1204 * 0.05));*/
    stallPWM = 0;//i;// * 1.2;
    averageMotorSpeed = 80;//(255 - stallPWM) * 0.2 + stallPWM;
    leftMotorCount = 0;
    rightMotorCount = 0;
    determineStallPWMDone = 1;

    //publishSyncData();
  }
}
