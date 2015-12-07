
#include "Planner.h"
#include "IntersectionPathfinder.h"
#include "Localization.h"
#include "Checkpoint.h"
#include "IntersectionDetection.h"
#include "LineFollower.h"
#include <math.h>
#include "Constants.h"
#include "Debug.h"
#include "LineFollower.h"
#include "Checkpoint.h"
#include "IntersectionPathfinder.h"
#include "IntersectionDetection.h"
#include "Localization.h"


void setup() {
  
  pinMode(AIN1_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN1_LEFT_MOTOR, OUTPUT);
  pinMode(AIN2_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN2_LEFT_MOTOR, OUTPUT);
  pinMode(IR_DETECTOR, INPUT);
  pinMode(ANTENNA_LED, OUTPUT);
  pinMode(WALL_DISTANCE_SENSOR, INPUT);
  pinMode(WALL_COLOUR_SENSOR, INPUT);
  Serial.begin(9600);

  //Initializing program variables
  lastError = 0;
  averageMotorSpeed = 75;
  stallPWM = 65;
  previousTime = millis();
  initializeEncoders();
  interrupts();
  wallTime = millis();
}

void loop() {

  delay(10); //delay is for PID to work smoother
  unsigned long currentTime;
  currentTime = millis();

  checkPointHandle(currentTime); //handles checkpoint detection 
  updateFollowerState(currentTime); //changes the state of the follower 
  ReadIntersectionSensors(currentTime); //Reads the intersection sensors and calls other functions to process them
  followLaneAnalog(currentTime); //calls PID lane control function
  wallDetection(currentTime); //calls wall detection function
  updateRelativeLocation(); //updates the location of the rover based on encoder counts
  publishEncoderData(leftMotorCount, rightMotorCount); //sends data to serial
  publishLocalizationData(currentTime); //sends data to serial


  while (Serial.available() > 0) {
    int command = Serial.read();
    processDebugCommand(command);
  }
}
