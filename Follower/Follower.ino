
#include <math.h>
#include "Constants.h"
#include "MotorController.h"
#include "Mapping.h"
#include "Pathfinder.h"


/*************Checkpoint Variables and Functions***********/
bool checkPointFound = false; //Once Robot finds the checkpoint, gets set to "true"
bool checkPointComplete = false; //Once the checkPointHandle() is complete after CHECKPOINT_REACTION_DURATION time, gets set to "true"
void checkPointHandle(int currentTime); //Function to handle the events that occur once checkpoint is found
void enableLED(); //turns checkpoint found indicator LED on
void disableLED(); //turns checkpoint found indicator LED off
int checkpointTime; //time in millis() at which the checkpoint was found. Used for  checkPointHandle() timer.
int ledBlinkCount = 0; //a counter to control the blinking of the LED
bool ledOn = false; //variable stoes the state of the LED.

/*************Lane following PID Variables and Functions***********/
void followLaneAnalog();
float lastError;
int integral;
void driveMotorsPID(int speedOffset);
int getLaneError();
int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
int stallPWM;//PWM at which the motor stalls

/*************Helper functions***********/
int roundToHundreds(int num); //rounds an integer to 100's
int signOf(float num);

void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN1_LEFT_MOTOR, OUTPUT);
  pinMode(AIN2_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN2_LEFT_MOTOR, OUTPUT);
  pinMode(IR_DETECTOR, INPUT);
  pinMode(ANTENNA_LED, OUTPUT);
  lastError = 0;
  averageMotorSpeed = 0;
  Serial.begin(115200);
  stallPWM = 55;

  //MoveRight();


  //SetMapElement(0, 0, true);

  //Serial.println(AccessMapElement(1, 0));
}

void loop() {
  //put your main code here, to run repeatedly:
  //Serial.print("Digital: ");
  //Serial.println(digitalRead(IR_DETECTOR));
  int currentTime = millis();
  checkPointHandle(currentTime);
}


/**************************************************************************************
*********************************Checkpoint Functions**********************************
***************************************************************************************/
void enableLED() {
  digitalWrite(ANTENNA_LED, HIGH);
  Serial.println("ON");
  ledOn = true;
}
void disableLED() {
  digitalWrite(ANTENNA_LED, LOW);
  Serial.println("OFF");
  ledOn = false;
}
void checkPointHandle(int currentTime) {
  //what the robot will do when it reaches the checkponint
  if (!digitalRead(IR_DETECTOR) && !checkPointFound) {
    checkPointFound = true;
    checkpointTime = currentTime;
  }
  //following may not work if loop takes longer than 100ms
  if (currentTime < (checkpointTime + CHECKPOINT_REACTION_DURATION) && checkPointFound && !checkPointComplete) {

    if (roundToHundreds(currentTime) == roundToHundreds(checkpointTime + ledBlinkCount * 600)) {
      if (!ledOn) {
        enableLED();
      }
    }

    else if (roundToHundreds(currentTime) == roundToHundreds(checkpointTime + (ledBlinkCount * 600) + 300)) {
      if (ledOn) {
        disableLED();
        ledBlinkCount++;
      }
    }

  }
  else if (ledOn) {
    disableLED();
    checkPointComplete = true;
    Serial.println("DONE");
  }
}

/**************************************************************************************
*********************************Lane following Functions******************************
***************************************************************************************/
void followLaneAnalog() {
  int derivative;
  float currentError = getLaneError(); //robot too far left is negative...too far right is positive
  float controller;


  integral = integral + currentError;
  derivative = currentError - lastError;
  lastError = currentError;
  controller = Kp * currentError + Ki * integral + Kd * derivative;
  driveMotorsPID(controller);

}


void driveMotorsPID(int controller) {
  //should make avg speed inversly proportional to the controller...will slow down if error is high
  float speedOffset = (controller * (averageMotorSpeed - (stallPWM - 10)) / 210); //controller offset is scaled with average speed (255-45). Cutoff at 45 PWM.
                      int newLeftMotorSpeed = averageMotorSpeed - speedOffset;
  int newRightMotorSpeed = averageMotorSpeed + speedOffset;

  //Controls what to do if the averageMotorSpeed is too low
  //Function will assume the car is stopped and will try to keep the car centered with the line
  if (averageMotorSpeed < stallPWM) {
    if (abs(lastError) > 50) {
      newLeftMotorSpeed = -signOf(controller) * (stallPWM + 5);
      newRightMotorSpeed = signOf(controller) * (stallPWM + 5);
    }
    else {
      newLeftMotorSpeed = 0;
      newRightMotorSpeed = 0;
    }

  }

  //checks if any of the motors exceed max PWM and carries the difference to the other motor.
  if (newLeftMotorSpeed > 255 || newRightMotorSpeed > 255) {
    int tempNum;
    if (newLeftMotorSpeed > 255) {
      tempNum = newLeftMotorSpeed - 255;
      newLeftMotorSpeed = 255;
      newRightMotorSpeed = newRightMotorSpeed - tempNum;
    }
    else if (newRightMotorSpeed > 255) {
      tempNum = newRightMotorSpeed - 255;
      newRightMotorSpeed = 255;
      newLeftMotorSpeed = newLeftMotorSpeed - tempNum;
    }

  }

  //checks if any of the motors are below stall PWM and sets them to zero
  if (newLeftMotorSpeed < stallPWM || newRightMotorSpeed < stallPWM) {
    if (newLeftMotorSpeed < stallPWM) {
      newLeftMotorSpeed = 0;
    }
    else if (newRightMotorSpeed < stallPWM) {
      newRightMotorSpeed = 0;
    }
  }

  //next 4 if statements drive the left and right motors forward or back depending on the signs of newLeftMotorSpeed and newRightMotorSpeed
  if (newLeftMotorSpeed >= 0) {
    analogWrite(BIN2_LEFT_MOTOR, 0);
    analogWrite(BIN1_LEFT_MOTOR, newLeftMotorSpeed);//drives left motor forward
  }
  else {
    analogWrite(BIN1_LEFT_MOTOR, 0);
    analogWrite(BIN2_LEFT_MOTOR, -newLeftMotorSpeed);//drives left motor reverse
  }

  if (newRightMotorSpeed >= 0) {
    analogWrite(AIN2_RIGHT_MOTOR, 0);
    analogWrite(AIN1_RIGHT_MOTOR, newRightMotorSpeed);//drives right motor forward
  }
  else {
    analogWrite(AIN1_RIGHT_MOTOR, 0);
    analogWrite(AIN2_RIGHT_MOTOR, -newRightMotorSpeed);//drives right motor reverse
  }
}

//Function returns error of car's position relative to the lane
int getLaneError() {
  int leftSensorValue = analogRead(LINE_FOLLOW_SENSOR_LEFT);
  int rightSensorValue = analogRead(LINE_FOLLOW_SENSOR_RIGHT);
  return leftSensorValue - rightSensorValue;
}
/**************************************************************************************
*************************************Helper Functions**********************************
***************************************************************************************/
int roundToHundreds(int num) {
  return round(num / 100) * 100;
}

//returns sign (+/-) of a number
//returns 1 if positive or 0
//returns -1 if negative
int signOf(float num) {
  if (num >= 0) return 1;
  else return -1;
}



