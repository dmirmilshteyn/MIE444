#ifndef _LINEFOLLOWER_h
#define _LINEFOLLOWER_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Constants.h"
#include "IntersectionDetection.h"
#include "Debug.h"
#include "Localization.h"
#include "Checkpoint.h"

#define FOLLOWER_STATE_ONLINE 0
#define FOLLOWER_STATE_OFFLINE 1
#define FOLLOWER_STATE_LEFT 2
#define FOLLOWER_STATE_RIGHT 3
#define FOLLOWER_STATE_REALIGN 4
#define FOLLOWER_STATE_IDENTIFY_WALL 5
#define FOLLOWER_STATE_WALL_DEADEND 6
#define FOLLOWER_STATE_WALL_START_DONE 7
#define FOLLOWER_STATE_WALL_START_GOBACK 8
#define FOLLOWER_STATE_STRAIGHT 9

#define TURN_STATE_DEFAULT 0
#define TURN_STATE_HIT_WHITE 1
#define TURN_STATE_HIT_BLACK 2

#define PATH_STRAIGHT 0
#define PATH_LEFT 1
#define PATH_RIGHT 2

extern int followerState;
extern bool isRealigning;

extern unsigned long previousTime;
extern float readLeft;
extern float readRight;

extern float lastError;
extern long integral;
extern bool leftForward;
extern bool rightForward;

extern int wallSensorBuffer;
extern int wallBufferCounter;
extern double wallDistance;

extern int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
extern int stallPWM; //PWM at which the motor stalls

extern int determineStallPWMDone;//states when determineStallPWMD() is complete

extern int currentLeftMotorSpeed;
extern int currentRightMotorSpeed;

void updateFollowerState(unsigned long currentTime);
MotorSpeeds driveMotorsBasic(float controller, float adjustedSpeed, float speedOffset);
MotorSpeeds driveMotorsPID(float controller, float derivative);

float getLaneError();
void followLaneAnalog(unsigned long currentTime);
void determineStallPWM();//will determine the stallPWM of the robot with the current payload and battery power. It will add speed to the motors until the robot starts moving.
void wallDetection(unsigned long currentTime);
void wallDetection2(unsigned long currentTime);

void driveMotorsAtSpeed(MotorSpeeds motorSpeeds);

#endif

