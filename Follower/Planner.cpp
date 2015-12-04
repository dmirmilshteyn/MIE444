#include "Planner.h"

// All possible destination points to be traveled
target targets[TARGET_COUNT] = {
	{ 9 },
	{ 7 },
	{ 5 },
	{ 3 },
	{ 14 }
};

IntersectionPathfinderResult currentPathPlan;

void ProcessDetectedIntersection(int detectedIntersectionType) {
	if (currentPath == -1) {
		pushDetectedIntersection(detectedIntersectionType);
	}
	else if (currentPath > -1) {
		updateIntersectionLocalization(detectedMarkerId);
	}

	switch (detectedIntersectionType) {
	case INTERSECTION_TYPE_LEFTTURN:
		followerState = FOLLOWER_STATE_LEFT;
		break;
	case INTERSECTION_TYPE_RIGHTTURN:
		followerState = FOLLOWER_STATE_RIGHT;
		break;
	case INTERSECTION_TYPE_T:
		followerState = FOLLOWER_STATE_RIGHT;
		break;
	case INTERSECTION_TYPE_TRIGHT:
		followerState = FOLLOWER_STATE_RIGHT;
		break;
	case INTERSECTION_TYPE_TLEFT:
		followerState = FOLLOWER_STATE_LEFT;
		break;
	}

#ifdef NOMOTORS
	followerState == FOLLOWER_STATE_ONLINE;
	detectedIntersection = INTERSECTION_TYPE_NONE;

	lastIntersectionDetectionLeftEncoder = leftMotorCount;
	lastIntersectionDetectionRightEncoder = rightMotorCount;
#endif
}

void BuildPathPlan() {
	// Find candidate paths for each target - pick the one that has the lowest cost
	IntersectionPathfinderResult *bestPath = NULL;

	IntersectionPathfinder pathfinder(absoluteHeadingAngle);
	for (int i = 0; i < TARGET_COUNT; i++) {
		if (!targets[i].hit) {
			IntersectionPathfinderResult candidatePath = pathfinder.FindPath(lastIntersectionMarkerId, targets[i].id);
			if (candidatePath.size > 0 && candidatePath.cost > -1) {
				if (bestPath == NULL || candidatePath.cost < bestPath->cost) {
					bestPath = &candidatePath;
				}
			}
		}
	}

	if (bestPath != NULL) {
		currentPathPlan = *bestPath;
	}
}