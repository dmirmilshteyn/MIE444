
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
void followLaneAnalog(int currentTime);
float lastError;
float integral;
void driveMotorsPID(float controller, float derivative);
float getLaneError();
int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
int stallPWM;//PWM at which the motor stalls

int previousTime;

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
  averageMotorSpeed = 80;
  Serial.begin(9600);
  stallPWM = 65;
  previousTime = millis();

  //MoveRight();


  //SetMapElement(0, 0, true);

  //Serial.println(AccessMapElement(36, 4));
  //Serial.println(AccessMapElement(20, 4));

  Pathfinder pathfinder;
  PathfinderResult path = pathfinder.FindPath(Position(47, 19), Position(47, 10));

  Serial.println("Values:");
  for (int i = 0; i < path.size; i++) {
    Serial.println(path.path[i]->y);
  }

//  Expected output: 
//  19
//  18
//  17
//  16
//  15
//  14
//  13
//  12
//  11
//  10

  Serial.print("Path Size: ");
  Serial.println(path.size);
}

void loop() {
  //put your main code here, to run repeatedly:
  //Serial.print("Digital: ");
  //Serial.println(digitalRead(IR_DETECTOR));
  delay(10);
  int currentTime;


  currentTime = millis();
  //checkPointHandle(currentTime);
  followLaneAnalog(currentTime);





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
void followLaneAnalog(int currentTime) {
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
  driveMotorsPID(controller, derivative);
  Serial.print("P:  ");
  Serial.print(Kp * currentError);
  Serial.print("    I:  ");
  Serial.print(Ki * integral);
  Serial.print("    D:  ");
  Serial.print(Kd * derivative);
  Serial.print("    Controller:   ");
  Serial.println(controller);
  previousTime = currentTime;
}


void driveMotorsPID(float controller, float derivative) {
  //should make avg speed inversely proportional to the controller...will slow down if error is high

  float adjustedSpeed = averageMotorSpeed-DERIVATIVE_SPEED_ADJUST*derivative*(averageMotorSpeed - (stallPWM)) / (255 - stallPWM);
  float speedOffset = abs((controller * (adjustedSpeed - (stallPWM)) / (255 - stallPWM))); //controller offset is scaled with average speed (255-stallPWM). Cutoff at stallPWM.
  int newLeftMotorSpeed;
  int newRightMotorSpeed;

  if (controller <= 0) {
    newLeftMotorSpeed = adjustedSpeed;
    newRightMotorSpeed = adjustedSpeed - speedOffset;
  }
  else if (controller > 0) {
    newLeftMotorSpeed = adjustedSpeed - speedOffset;
    newRightMotorSpeed = adjustedSpeed;
  }

  //Controls what to do if the adjustedSpeed is too low
  //Function will assume the car is stopped and will try to keep the car centered with the line
  if (adjustedSpeed < stallPWM) {
    if (abs(lastError) > 50) {
      newLeftMotorSpeed = -signOf(controller) * (stallPWM + 10);
      newRightMotorSpeed = signOf(controller) * (stallPWM + 10);
    }
    else {
      newLeftMotorSpeed = 0;
      newRightMotorSpeed = 0;
    }

  }
  //checks if any of the motors are below stall PWM and sets them to zero
  if (abs(newLeftMotorSpeed) < stallPWM || abs(newRightMotorSpeed) < stallPWM) {
    if (abs(newLeftMotorSpeed) < stallPWM) {
      newLeftMotorSpeed = 0;
    }
    else if (abs(newRightMotorSpeed) < stallPWM) {
      newRightMotorSpeed = 0;
    }
  }

  //stops the car if it left the line (on white)
  if (analogRead(LINE_FOLLOW_SENSOR_LEFT) < 750 && analogRead(LINE_FOLLOW_SENSOR_RIGHT) < 750) {
    newLeftMotorSpeed = 0;
    newRightMotorSpeed = 0;

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
float getLaneError() {
  float leftSensorValue = analogRead(LINE_FOLLOW_SENSOR_LEFT);
  float rightSensorValue = analogRead(LINE_FOLLOW_SENSOR_RIGHT);
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



