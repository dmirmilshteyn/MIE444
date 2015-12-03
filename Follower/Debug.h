#ifndef DEBUG_H
#define DEBUG_H

#include "Arduino.h"
#include "LineFollower.h"
#include "Core.h"
#include "Localization.h"
#include "IntersectionPathfinder.h"

//#define PRINT_DEBUG_MESSAGES
//#define PRINT_DEBUG_MESSAGES_2
//#define NOMOTORS

// ************************
// **** Debug Commands ****
// ************************
#define DEBUG_REQUEST_SYNC '!'
#define DEBUG_SYNC_MAP 'M'
#define DEBUG_OUTPUT_STATE '0'

#define DEBUG_SET_P '1'
#define DEBUG_SET_I '2'
#define DEBUG_SET_D '3'

#define DEBUG_MOVE_FORWARD 'w'
#define DEBUG_MOVE_LEFT 'a'
#define DEBUG_MOVE_RIGHT 'd'
#define DEBUG_MOVE_BACKWARD 's'

#define DEBUG_SPEED_ADJUST 'z'
#define DEBUG_AVERAGE_SPEED 'x'
#define DEBUG_STALL_PWM 'y'

extern int averageMotorSpeed;

inline void print(const char* message);
inline void print(float value);
inline void println();
inline void println(const char* message);
inline void println(float value);

int readDebugCommand();

void processDebugCommand(int command);

void publishLaneFollowingData(long currentTime, MotorSpeeds motorSpeeds, float currentError, float integral, float derivative, float controller, float leftLineSensor, float rightLineSensor);
void publishIntersectionDetectionData(long currentTime, int frontSensor, int leftSensor, int rightSensor, int detectedIntersection);
void publishEncoderData(long leftMotorCount, long rightMotorCount);
void publishMap();
void publishLocalizationData(long currentTime);
void publishPathInformation(long currentTime, IntersectionPathfinderResult path);
void publishCurrentPathPlanNodeIndex(int currentPathPlanNodeIndex);

#endif
