#include "MappingConstants.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**********************************************
 ************* Hardware Constants *************
 **********************************************/

 //DIGITAL PINS
#define AIN1_RIGHT_MOTOR 3 //FORWARD
#define AIN2_RIGHT_MOTOR 5 //REVERSE
#define BIN1_LEFT_MOTOR 9 //FORWARD
#define BIN2_LEFT_MOTOR 6 //REVERSE
#define WALL_SENSOR_1 2
#define IR_DETECTOR 4
#define ANTENNA_LED 7

//ANALOG PINS
#define LINE_FOLLOW_SENSOR_LEFT 0
#define LINE_FOLLOW_SENSOR_RIGHT 1
#define LINE_SENSOR_1 2
#define LINE_SENSOR_2 3
#define LINE_SENSOR_3 4
#define WALL_DISTANCE_SENSOR 5






/**********************************************
 *********** Calibration Constants ************
 **********************************************/

#define CHECKPOINT_REACTION_DURATION 5000

 //PID
 //#define Kp 3.4
 //#define Ki 0.000
 //#define Kd 400

static float Kp = 3.4;
static float Ki = 0.00;
static float Kd = 400;

#define DERIVATIVE_SPEED_ADJUST 1

//best settings for pwm 65
//#define Kp 1.7
//#define Ki 0.00
//#define Kd 200


#endif
