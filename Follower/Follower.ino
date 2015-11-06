
#include <math.h>
#include "Constants.h"
#include "MotorController.h"
#include "Mapping.h"


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
void followLane();
float lastError;
int integral;
void driveMotorsPID(int speedOffset)

/*************Helper functions***********/
int roundToHundreds(int num); //rounds an integer to 100's


void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(IR_DETECTOR, INPUT);
  pinMode(ANTENNA_LED, OUTPUT);
  lastError = 0;

  Serial.begin(115200);

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
void followLane() {
  int leftSensor = 1 - digitalRead(LINE_FOLLOW_SENSOR_LEFT);
  int rightSensor = 1 - digitalRead(LINE_FOLLOW_SENSOR_RIGHT);
  int derivative;
  float currentError; //too far left is negative...too far right is positive
  float controller;

  if (leftSensor || rightSensor) {
    if (leftSensor && rightSensor) {
      if (lastError < 0) {
        currentError = -(leftSensor + rightSensor);
      }
      else {
        currentError = (leftSensor + rightSensor);
      }
    }
    else if (leftSensor) {
      currentError = -leftSensor;
    }
    else if (rightSensor) {
      currentError = rightSensor;
    }
  }
  else {
    currentError = 0;
  }
  integral = integral+currentError;
  derivative = currentError-lastError;


  controller = Kp * currentError + Ki * integral + Kd * derivative;
  driveMotorsPID(controller)

  lastError = currentError;
}
void driveMotorsPID(int speedOffset){

  analogWrite(AIN1, AVERAGE_MOTOR_SPEED+speedOffset);
  analogWrite(BIN1, AVERAGE_MOTOR_SPEED-speedOffset);
}

/**************************************************************************************
*************************************Helper Functions**********************************
***************************************************************************************/
int roundToHundreds(int num) {
  return round(num / 100) * 100;
}





