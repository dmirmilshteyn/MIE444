#include "Planner.h"

// All possible destination points to be traveled
target targets[TARGET_COUNT] = {
	{ 9 },
	{ 7 },
	{ 5 },
	{ 3 },
	{ 14 }
};

PathPlan currentPathPlan;

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

	// An active path plan is loaded, attempt to follow it
	if (currentPathPlan.path.size > 0) {
		if (currentPathPlan.pathIndex == currentPathPlan.path.size - 1) {
			// Arrived at the destination, attempt to enter the branch
			// Mark the target as hit
			targets[currentPathPlan.target].hit = true;

			// Reset the current path plan
			currentPathPlan = PathPlan();

			// TODO: What happens after the target has been hit?
			followerState = FOLLOWER_STATE_WALL_START_DONE;
		}
		else {
			// Currently enroute to the destination, travel in the next direction
			int currentTurn = (int)currentPathPlan.path.pathTurns[currentPathPlan.pathIndex];

			switch (currentTurn) {
			case PATH_STRAIGHT:
				followerState = FOLLOWER_STATE_ONLINE;
				break;
			case PATH_LEFT:
				followerState = FOLLOWER_STATE_LEFT;
				break;
			case PATH_RIGHT:
				followerState = FOLLOWER_STATE_RIGHT;
				break;
			}

			currentPathPlan.pathIndex++;
		}
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
	IntersectionPathfinderResult bestPath;
	int selectedTarget = -1;

	IntersectionPathfinder pathfinder(absoluteHeadingAngle);
	for (int i = 0; i < TARGET_COUNT; i++) {
		if (!targets[i].hit) {
			IntersectionPathfinderResult candidatePath = pathfinder.FindPath(lastIntersectionMarkerId, targets[i].id);
			if (candidatePath.size > 0 && candidatePath.cost > -1) {
				if (bestPath.size == 0 || candidatePath.cost < bestPath.cost) {
					bestPath = candidatePath;
					selectedTarget = i;
				}
			}
		}
	}

	if (bestPath.size > 0) {
		currentPathPlan = PathPlan(bestPath, selectedTarget);
	}
}