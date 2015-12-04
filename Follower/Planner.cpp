#include "Planner.h"

// All possible destination points to be traveled
target targets[TARGET_COUNT] = {
	/*{ 9 },
	{ 7 },
	{ 5 },*/
	{ 3 }
	/*{ 14 }*/
};

PathPlan currentPathPlan;

void ProcessDetectedIntersection(int detectedIntersectionType) {
	Serial.println("DETECTED");

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

	int lastLeftSpeed = currentLeftMotorSpeed;
	int lastRightSpeed = currentRightMotorSpeed;

	Serial.print("Detected: ");
	Serial.println(lastIntersectionMarkerId);

	//driveMotorsAtSpeed(MotorSpeeds(0, 0));

	//delay(5000);

	//driveMotorsAtSpeed(MotorSpeeds(lastLeftSpeed, lastRightSpeed));


#ifndef NOPATHPLANFOLLOW
	// An active path plan is loaded, attempt to follow it
	if (currentPathPlan.path.size > 0) {
		publishCurrentPathPlanNodeIndex(currentPathPlan.pathIndex);
		if (currentPathPlan.pathIndex == currentPathPlan.path.size - 1) {
			// Arrived at the destination, attempt to enter the branch
			// Mark the target as hit
			targets[currentPathPlan.target].hit = true;

			// Find the closest start position to this target
			int targetStartPosition = -1;
			int currentIntersectionId = pgm_read_byte(&(intersections[(int)currentPathPlan.path.path[currentPathPlan.pathIndex]].id));
			for (int i = 0; i < STARTING_POSITION_COUNT; i++) {
				if (pgm_read_byte(&(start_positions[i].nearestIntersectionId)) == currentIntersectionId) {
					targetStartPosition = i;
					break;
				}
			}

			if (targetStartPosition != -1) {
				// If a target start position was found, determine the turn direction
				byte upcomingX = pgm_read_byte(&(start_positions[targetStartPosition].x));
				byte upcomingY = pgm_read_byte(&(start_positions[targetStartPosition].y));

				byte currentX = pgm_read_byte(&(intersections[(int)currentPathPlan.path.path[currentPathPlan.pathIndex]].intersectionX));
				byte currentY = pgm_read_byte(&(intersections[(int)currentPathPlan.path.path[currentPathPlan.pathIndex]].intersectionY));

				int currentTurn = DetermineTurnDirection(absoluteHeadingAngle, currentX, currentY, upcomingX, upcomingY);

				switch (currentTurn) {
				case PATH_STRAIGHT:
					followerState = FOLLOWER_STATE_STRAIGHT;
					break;
				case PATH_LEFT:
					followerState = FOLLOWER_STATE_LEFT;
					break;
				case PATH_RIGHT:
					followerState = FOLLOWER_STATE_RIGHT;
					break;
				}

				followerState = FOLLOWER_STATE_WALL_START_DONE;
			}
			else {
				// TODO: What happens when the target has been hit and it is not near a start position?

				// For now, end the robot movement
				followerState = FOLLOWER_STATE_WALL_START_DONE;
			}

			// Reset the current path plan once everything is done
			currentPathPlan = PathPlan();
		}
		else {
			// Currently enroute to the destination, travel in the next direction
			int currentTurn = (int)currentPathPlan.path.pathTurns[currentPathPlan.pathIndex];

			switch (currentTurn) {
			case PATH_STRAIGHT:
				followerState = FOLLOWER_STATE_STRAIGHT;
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
#endif // ! NOPATHPLANFOLLOW

#ifdef NOMOTORS
	followerState == FOLLOWER_STATE_ONLINE;
	detectedIntersection = INTERSECTION_TYPE_NONE;

	lastIntersectionDetectionLeftEncoder = leftMotorCount;
	lastIntersectionDetectionRightEncoder = rightMotorCount;
#endif
}

void BuildPathPlan() {
	int lastLeftSpeed = currentLeftMotorSpeed;
	int lastRightSpeed = currentRightMotorSpeed;

	driveMotorsAtSpeed(MotorSpeeds(0, 0));

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

		publishPathInformation(-1, currentPathPlan.path);
	}

	driveMotorsAtSpeed(MotorSpeeds(lastLeftSpeed, lastRightSpeed));
}