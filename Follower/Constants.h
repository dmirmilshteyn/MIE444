#include "MappingConstants.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**********************************************
 ************* Hardware Constants *************
 **********************************************/
#define AIN1_RIGHT_MOTOR 3
#define AIN2_RIGHT_MOTOR 5
#define BIN1_LEFT_MOTOR 9
#define BIN2_LEFT_MOTOR 6

#define LINE_SENSOR_1 2 //digital
#define LINE_SENSOR_2 4 //digital
#define LINE_SENSOR_3 7 //digital
//#define LINE_FOLLOW_SENSOR_LEFT 8 //digital
//#define LINE_FOLLOW_SENSOR_RIGHT 12 //digital
#define WALL_SENSOR_1 11 //digital
#define LINE_FOLLOW_SENSOR_LEFT 0 //analog
#define LINE_FOLLOW_SENSOR_RIGHT 1 //analog

#define IR_DETECTOR 10 //digital

#define ANTENNA_LED 13 //digital

/**********************************************
 *********** Calibration Constants ************
 **********************************************/

#define CHECKPOINT_REACTION_DURATION 5000

//PID
#define LINE_DETECTED 1
#define LINE_NOT_DETECTED 0
#define Kp 0.3
#define Ki 0.1
#define Kd 1


#endif
