/**********************************************
 ** This is a generated file. Do not modify. **
 **********************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**********************************************
 ************* Hardware Constants *************
 **********************************************/
const int AIN1 = 3;
const int AIN2 = 5;
const int BIN1 = 9;
const int BIN2 = 6;

const int LINE_SENSOR_1 = 2; //digital
const int LINE_SENSOR_2 = 4; //digital
const int LINE_SENSOR_3 = 7; //digital
const int LINE_FOLLOW_SENSOR_LEFT = 8; //digital
const int LINE_FOLLOW_SENSOR_RIGHT = 12; //digital
const int WALL_SENSOR_1 = 11; //digital

const int IR_DETECTOR = 10; //digital

const int ANTENNA_LED = 13; //digital

/**********************************************
 ************** Robot Constants ***************
 **********************************************/
const float WHEEL_RADIUS = 0.035;

/**********************************************
 *********** Environment Constants ************
 **********************************************/
const float LINE_WIDTH = 0.05;
const int MAP_WIDTH = 5;
const int MAP_HEIGHT = 5;
const float MAP_RESOLUTION = 0.025;


/**********************************************
 *********** Calibration Constants ************
 **********************************************/

const int CHECKPOINT_REACTION_DURATION = 5000;

//PID
const bool LINE_DETECTED = 1;
const bool LINE_NOT_DETECTED = 0;
const float Kp = 1;
const float Ki = 0.1;
const float Kd = 0.5;
const int AVERAGE_MOTOR_SPEED = 130;//avg PWM for both motors. Might make this a variable in the future


#endif
