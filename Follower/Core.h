#ifndef _CORE_h
#define _CORE_h

#include <math.h>

struct MotorSpeeds {
	int left;
	int right;

	MotorSpeeds() : left(0), right(0) {}
	MotorSpeeds(int left, int right) : left(left), right(right) {}
};

//rounds an integer to 100's
#define roundToHundreds(num) \
	round(num / 100) * 100

//returns sign (+/-) of a number
//returns 1 if positive or 0
//returns -1 if negative
#define signOf(num) \
	(num >= 0) ? 1 : -1

double normalise(const double value, const double start, const double end);

#endif