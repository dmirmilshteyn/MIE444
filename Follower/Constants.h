#include "MappingConstants.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**********************************************
 ************* Hardware Constants *************
 **********************************************/

//DIGITAL PINS
#define AIN1_RIGHT_MOTOR 10 //FORWARD
#define AIN2_RIGHT_MOTOR 5 //REVERSE
#define BIN1_LEFT_MOTOR 9 //FORWARD
#define BIN2_LEFT_MOTOR 6 //REVERSE
#define WALL_COLOUR_SENSOR 12
#define IR_DETECTOR 4
#define ANTENNA_LED 7

#define ENCODER_LEFT_MOTOR 2
#define ENCODER_LEFT_MOTORB 11
#define ENCODER_RIGHT_MOTOR 3
#define ENCODER_RIGHT_MOTORB 8

//ANALOG PINS
#define LINE_FOLLOW_SENSOR_LEFT 0
#define LINE_FOLLOW_SENSOR_RIGHT 1
#define LINE_SENSOR_1 2 //front
#define LINE_SENSOR_2 3 //left
#define LINE_SENSOR_3 4 //right
#define WALL_DISTANCE_SENSOR 5

/**********************************************
 *********** Calibration Constants ************
 **********************************************/
#define CHECKPOINT_REACTION_DURATION 5000
#define GEAR_RATIO 297.92
#define ENCODER_TEETH_COUNT 6
#define CHECKPOINTS_TOTAL 2 //total number of checkpoints in the maze

/**********************************************
 ************** Robot Constants ***************
 **********************************************/
#define WHEEL_RADIUS 0.035
#define ROBOT_WIDTH 0.21

#define WHEEL_GAP 0.2035


#endif
