#ifndef _PLANNER_h
#define _PLANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LineFollower.h"

void ProcessDetectedIntersection(int detectedIntersectionType);

#endif

