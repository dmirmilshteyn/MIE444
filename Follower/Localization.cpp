#include "Localization.h"

volatile long int leftMotorCount;
volatile long int rightMotorCount;
long int previousLeftMotorCount = 0;
long int previousRightMotorCount = 0;
int absoluteLocationX = -1;
int absoluteLocationY = -1;
double absoluteLocationXMeters = 0; //************ADD*************
double absoluteLocationYMeters = 0; //************ADD*************
double absoluteHeadingAngle = 0; //rads...right is positive, left is negative
void updateRelativeLocation();
void correctRelativeAngle();
bool correctRelativeAngleDone = false;

int currentPath = -1;
int lastIntersectionMarkerId = -1;

// Absolute localization based on detected intersections
int intersectionChain[MAX_PATH_LENGTH] = { 0 };
int nextIntersectionIndex = 0;

// Available, unique paths that are mapped to physical locations
int intersectionPaths[PATH_COUNT][MAX_PATH_LENGTH] = {
  { INTERSECTION_TYPE_TRIGHT, -1 },
  { INTERSECTION_TYPE_TLEFT, -1 },
  { INTERSECTION_TYPE_T, INTERSECTION_TYPE_LEFTTURN },
  { INTERSECTION_TYPE_T, INTERSECTION_TYPE_TLEFT },
  { INTERSECTION_TYPE_T, INTERSECTION_TYPE_TRIGHT }
};

// Array is stored in the following structure:
// { Intersection Id, Absolute Heading }
float pathLocation[PATH_COUNT][MAX_PATH_LENGTH + 1] = {
  { 14, -1, M_PI / 2 }, // Starting with TRight
  { 21, -1, -M_PI / 2 }, // Starting with TLeft
  { 39, 38,  M_PI / 4 }, // Starting with T, then Left
  { 39, 38,  M_PI / 4 }, // Starting with T, then TLeft (this is an error case)
  { 25, 24, 0 } // Starting with T, then TRight
};

void initializeEncoders() {
  pinMode(ENCODER_LEFT_MOTOR, INPUT);
  digitalWrite(ENCODER_LEFT_MOTOR, HIGH);
  pinMode(ENCODER_LEFT_MOTORB, INPUT);
  digitalWrite(ENCODER_LEFT_MOTORB, HIGH);


  pinMode(ENCODER_RIGHT_MOTOR, INPUT);
  digitalWrite(ENCODER_RIGHT_MOTOR, HIGH);
  pinMode(ENCODER_RIGHT_MOTORB, INPUT);
  digitalWrite(ENCODER_RIGHT_MOTORB, HIGH);

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_MOTOR), handleLeftMotorInterupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_MOTOR), handleRightMotorInterupt, CHANGE);
}

void handleLeftMotorInterupt() {
  // Note: 1 wheel rotation = GEAR_RATIO * ENCODER_TEETH_COUNT = 100.37 * 12 = 1204.44 clicks
  /*if (leftForward == true) {
    leftMotorCount++;
    }
    else {
    leftMotorCount--;
    }*/
  /*if(digitalRead(ENCODER_LEFT_MOTORB)) leftMotorCount++;
    else leftMotorCount--;*/

  if (digitalRead(ENCODER_LEFT_MOTOR) == digitalRead(ENCODER_LEFT_MOTORB)) {
    leftMotorCount--;
  }
  else {
    leftMotorCount++;
  }

  // TODO: Handle direction
}

void handleRightMotorInterupt() {
  /*if (rightForward == true) {
    rightMotorCount++;
    }
    else {
    rightMotorCount--;
    }*/
  /*if(digitalRead(ENCODER_RIGHT_MOTORB)) rightMotorCount++;
    else leftMotorCount--;*/

  if (digitalRead(ENCODER_RIGHT_MOTOR) == digitalRead(ENCODER_RIGHT_MOTORB)) {
    rightMotorCount--;
  }
  else {
    rightMotorCount++;
  }

  // TODO: Handle direction
}

//if robot is placed at an angle with the line, this code will correct the angle and position after the robot has travelled 0.4m
void correctRelativeAngle() {
  double travelDistFromStart = sqrt(pow(absoluteLocationXMeters, 2) + pow(absoluteLocationYMeters, 2));
  double relativeCorrectionAngle;
  if (travelDistFromStart > 0.4) {
    relativeCorrectionAngle = atan(absoluteLocationYMeters / absoluteLocationXMeters);
    absoluteHeadingAngle -= relativeCorrectionAngle;
    absoluteLocationXMeters = travelDistFromStart;
    absoluteLocationYMeters = 0;
    absoluteLocationX = round(absoluteLocationXMeters / MAP_RESOLUTION);
    absoluteLocationY = round(absoluteLocationYMeters / MAP_RESOLUTION);
    correctRelativeAngleDone = true;
  }
}
void updateRelativeLocation() {
  double leftEncoderDiff = leftMotorCount - previousLeftMotorCount;
  previousLeftMotorCount = leftEncoderDiff + previousLeftMotorCount;
  double rightEncoderDiff = rightMotorCount - previousRightMotorCount;
  previousRightMotorCount = rightEncoderDiff + previousRightMotorCount;

  double absoluteHeadingAngleDiff;
  double travelDist;

  double leftEncoderDiffMeters = leftEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;
  double rightEncoderDiffMeters = rightEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;



  absoluteHeadingAngleDiff = (leftEncoderDiffMeters - rightEncoderDiffMeters) / WHEEL_GAP;

  //Serial.print(absoluteHeadingAngleDiff);

  if (abs(absoluteHeadingAngleDiff) > 0.001) {
    travelDist = abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) * sqrt(2 - 2 * cos(absoluteHeadingAngleDiff))) / (2 * absoluteHeadingAngleDiff));
  }
  else {
    travelDist = abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) / (2)));
  }

  absoluteLocationXMeters += cos(absoluteHeadingAngle + absoluteHeadingAngleDiff / 2) * travelDist;
  absoluteLocationYMeters += sin(absoluteHeadingAngle + absoluteHeadingAngleDiff / 2) * travelDist;

  absoluteLocationX = round(absoluteLocationXMeters / MAP_RESOLUTION);
  absoluteLocationY = round(absoluteLocationYMeters / MAP_RESOLUTION);

  absoluteHeadingAngle += absoluteHeadingAngleDiff;


  //Serial.print(" ");
  //Serial.println(absoluteHeadingAngle);
  //  if (correctRelativeAngleDone == false) {
  //    correctRelativeAngle();
  //  }
}

void pushDetectedIntersection(int intersectionType) {

  if (nextIntersectionIndex < MAX_PATH_LENGTH) {
    intersectionChain[nextIntersectionIndex++] = intersectionType;

    int result = tryToLocalize();
    if (result > -1) {
      // Localization was successful

      for (int i = 0; i < MAX_PATH_LENGTH; i++) {

        lastIntersectionMarkerId = pathLocation[result][i];
        if (pathLocation[result][i] == -1) {
          lastIntersectionMarkerId = pathLocation[result][i - 1];
          break;
        }

      }

	  // Hit all targets that are part of this path
	  for (int i = 0; i < TARGET_COUNT; i++) {
		  for (int p = 0; p < MAX_PATH_LENGTH; p++) {
			  if (pathLocation[result][p] != -1) {
				  int pathIntersectionid = pgm_read_byte(&(intersections[(int)pathLocation[result][p]].id));

				  if (targets[i].id == pathIntersectionid) {
					  targets[i].hit = true;
				  }
			  }
			  
		  }
	  }

	  // Build a path plan based on our current location
	  BuildPathPlan();
    }
  }
}

void updateIntersectionLocalization(int currentIntersectionMarkerId) {
  // Try to identify the current intersection marker id


  bool valid = false;
  int x = lastIntersectionMarkerId;
  double smallestDistance = 1000;
  int smallestDistanceMarkerID = -1;


  //angle and position correction
  // Calculate expected heading angle between the last and current intersection
  byte lastIntersectionX = pgm_read_byte(&(intersections[x].intersectionX));
  byte lastIntersectionY = pgm_read_byte(&(intersections[x].intersectionY));

  byte currentIntersectionX = pgm_read_byte(&(intersections[currentIntersectionMarkerId].intersectionX));
  byte currentIntersectionY = pgm_read_byte(&(intersections[currentIntersectionMarkerId].intersectionY));

  double xDiff = currentIntersectionX - lastIntersectionX;
  double yDiff = currentIntersectionY - lastIntersectionY;


  double angle = atan2(yDiff, xDiff);
  byte deadEndCase = 0;
  switch (currentIntersectionMarkerId) {
    case 3:
      angle = 0;
            deadEndCase = 1;
      break;
    case 25:
      angle = -M_PI / 2;
            deadEndCase = 1;
      break;
    case 21:
      angle = -M_PI / 2;
            deadEndCase = 1;
      break;
    case 14:
      angle = M_PI / 2;
            deadEndCase = 1;
      break;
    case 9:
      angle = -M_PI / 2;
            deadEndCase = 1;
      break;
    case 39:
      angle = -M_PI / 3;
            deadEndCase = 1;
      break;
    case 10: //arc line
      angle = -M_PI;
      break;
    case 11: //arc line
      angle = M_PI / 2;
      break;
    case 43: //arc line
      angle = -M_PI * 0.9;
      break;
    case 38: //arc line
      angle = M_PI / 3;
      break;
    case 7: //arc line
      angle = M_PI * 0.9;
      break;
    case 8: //arc line
      angle = 0;
      break;
  }

  // TODO: add special cases for angle where the line between 2 intersections isnt straight (i.e. arcs)

  absoluteHeadingAngle = angle;
  absoluteLocationX = currentIntersectionX;
  absoluteLocationY = currentIntersectionY;
  absoluteLocationXMeters = (double)absoluteLocationX * MAP_RESOLUTION;
  absoluteLocationYMeters = (double)absoluteLocationY * MAP_RESOLUTION;
  lastIntersectionMarkerId = currentIntersectionMarkerId;


  lastIntersectionEncoderTick = (leftMotorCount + rightMotorCount) / 2;

  if(deadEndCase == 1){
    processDeadEnd();
  }

  //  Serial.print(" angle");
  //  Serial.println(absoluteHeadingAngle * 180 / M_PI);
  //  Serial.println();
  //Serial.println(lastIntersectionMarkerId);
  //    Serial.print("   ");
  //    Serial.println(intersections[lastIntersectionMarkerId].id);
}

int tryToLocalize() {
  for (int p = 0; p < PATH_COUNT; p++) {

    bool pathValid = true;
    for (int i = 0; i < MAX_PATH_LENGTH; i++) {
      int currentIntersection = intersectionPaths[p][i];
      if (currentIntersection != -1) {
        if (currentIntersection != intersectionChain[i]) {
          pathValid = false;
          break;
        }
      }
    }

    if (pathValid) {
      currentPath = p;

      //corrects current angle and position of car
      absoluteHeadingAngle = pathLocation[p][MAX_PATH_LENGTH];

      for (int i = MAX_PATH_LENGTH - 1; i >= 0; i--) {

        if (pathLocation[p][i] != -1) {
          absoluteLocationX = pgm_read_byte(&(intersections[(int)pathLocation[p][i]].intersectionX));
          absoluteLocationY = pgm_read_byte(&(intersections[(int)pathLocation[p][i]].intersectionY));
          absoluteLocationXMeters = (double)absoluteLocationX * MAP_RESOLUTION;
          absoluteLocationYMeters = (double)absoluteLocationY * MAP_RESOLUTION;
          break;
        }

      }

      return currentPath;
    }
  }
  return -1;
}

int DetermineTurnDirection(double headingAngle, int currentX, int currentY, int upcomingX, int upcomingY) {
	double xDiff = upcomingX - currentX;
	double yDiff = upcomingY - currentY;

	double upcomingAngle = atan2(yDiff, xDiff);

	double previousAngle = headingAngle;

	double angle = normalise(upcomingAngle, 0, 2 * M_PI) - normalise(previousAngle, 0, 2 * M_PI);
	angle = normalise(angle, -M_PI, M_PI);

	if (abs(angle) < 20 * M_PI / 180) {
		return PATH_STRAIGHT;
	}
	else if (angle > 0) {
		return PATH_RIGHT;
	}
	else if (angle < 0) {
		return PATH_LEFT;
	}
}





