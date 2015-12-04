#ifndef _PLANNER_h
#define _PLANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LineFollower.h"
#include "IntersectionPathfinder.h"

#define TARGET_COUNT 1

struct target {
	int id;
	bool hit;

	target() : id(0), hit(false) {}
	target(int id) : id(id), hit(false) {}
};

struct PathPlan {
	IntersectionPathfinderResult path;
	int pathIndex;
	int target;

	PathPlan() : pathIndex(0), target(-1) {}
	PathPlan(IntersectionPathfinderResult path, int target) : path(path), pathIndex(0), target(target) {}
};

extern target targets[TARGET_COUNT];
extern PathPlan currentPathPlan;

void ProcessDetectedIntersection(int detectedIntersectionType);
void BuildPathPlan();

void processDeadEnd();

#endif

