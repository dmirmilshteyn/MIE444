#include "Planner.h"

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
