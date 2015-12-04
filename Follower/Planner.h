#ifndef _PLANNER_h
#define _PLANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LineFollower.h"
#include "IntersectionPathfinder.h"

#define TARGET_COUNT 5

struct target {
	int id;
	bool hit;

	target() : id(0), hit(false) {}
	target(int id) : id(id), hit(false) {}
};

extern target targets[TARGET_COUNT];
extern IntersectionPathfinderResult currentPathPlan;

void ProcessDetectedIntersection(int detectedIntersectionType);
void BuildPathPlan();

#endif

