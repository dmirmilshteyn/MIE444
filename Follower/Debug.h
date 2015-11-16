#include "Arduino.h"

#ifndef DEBUG_H
#define DEBUG_H

// ************************
// **** Debug Commands ****
// ************************
#define DEBUG_REQUEST_SYNC '!'
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

extern int averageMotorSpeed;

void print(const char* message);
void println(const char* message);

int readDebugCommand();

void processDebugCommand(int command);

#endif