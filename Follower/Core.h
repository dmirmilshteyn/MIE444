#ifndef _CORE_h
#define _CORE_h

struct MotorSpeeds {
	int left;
	int right;
};

//rounds an integer to 100's
#define roundToHundreds(num) \
	round(num / 100) * 100

//returns sign (+/-) of a number
//returns 1 if positive or 0
//returns -1 if negative
#define signOf(num) \
	(num >= 0) ? 1 : -1

#endif