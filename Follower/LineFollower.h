#ifndef _LINEFOLLOWER_h
#define _LINEFOLLOWER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"
#include "Debug.h"

extern int previousTime;
extern float readLeft;
extern float readRight;

extern float lastError;
extern float integral;

extern int averageMotorSpeed;//avg PWM for both motors. Value is variable to control intersections and lane stability
extern int stallPWM; //PWM at which the motor stalls

MotorSpeeds driveMotorsPID(float controller, float derivative);
float getLaneError();
void followLaneAnalog(int currentTime);


#endif

