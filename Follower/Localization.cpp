#include "Localization.h"

volatile long int leftMotorCount;
volatile long int rightMotorCount;
long int previousLeftMotorCount = 0;
long int previousRightMotorCount = 0;
int absoluteLocationX = -1;
int absoluteLocationY = -1;
double absoluteHeadingAngle = 0;
int relativeLocationX = 0;
int relativeLocationY = 0;
double relativeLocationXMeters = 0; //************ADD*************
double relativeLocationYMeters = 0; //************ADD*************
double relativeHeadingAngle = 0; //rads...right is negative, left is positive
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
  { 14, -1, 3 * M_PI / 2 }, // Starting with TRight
  { 21, -1, M_PI / 2 }, // Starting with TLeft
  { 39, 38, - M_PI / 4 }, // Starting with T, then Left
  { 39, 38, - M_PI / 4 }, // Starting with T, then TLeft (this is an error case)
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
  double travelDistFromStart = sqrt(pow(relativeLocationXMeters, 2) + pow(relativeLocationYMeters, 2));
  double relativeCorrectionAngle;
  if (travelDistFromStart > 0.4) {
    relativeCorrectionAngle = atan(relativeLocationYMeters / relativeLocationXMeters);
    relativeHeadingAngle -= relativeCorrectionAngle;
    relativeLocationXMeters = travelDistFromStart;
    relativeLocationYMeters = 0;
    relativeLocationX = round(relativeLocationXMeters / MAP_RESOLUTION);
    relativeLocationY = round(relativeLocationYMeters / MAP_RESOLUTION);
    correctRelativeAngleDone = true;
  }
}
void updateRelativeLocation() {
  double leftEncoderDiff = leftMotorCount - previousLeftMotorCount;
  previousLeftMotorCount = leftEncoderDiff + previousLeftMotorCount;
  double rightEncoderDiff = rightMotorCount - previousRightMotorCount;
  previousRightMotorCount = rightEncoderDiff + previousRightMotorCount;

  double relativeHeadingAngleDiff;
  double travelDist;

  double leftEncoderDiffMeters = leftEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;
  double rightEncoderDiffMeters = rightEncoderDiff / ENCODER_TEETH_COUNT / GEAR_RATIO * WHEEL_RADIUS * 2 * M_PI;



  relativeHeadingAngleDiff = (rightEncoderDiffMeters - leftEncoderDiffMeters) / WHEEL_GAP;

  //Serial.print(relativeHeadingAngleDiff);

  if (abs(relativeHeadingAngleDiff) > 0.001) {
    travelDist = abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) * sqrt(2 - 2 * cos(relativeHeadingAngleDiff))) / (2 * relativeHeadingAngleDiff));
  }
  else {
    travelDist = abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) / (2)));
  }

  relativeLocationXMeters += cos(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;
  relativeLocationYMeters += sin(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;

  relativeLocationX = round(relativeLocationXMeters / MAP_RESOLUTION);
  relativeLocationY = round(relativeLocationYMeters / MAP_RESOLUTION);

  relativeHeadingAngle += relativeHeadingAngleDiff;

  //Serial.print(" ");
  //Serial.println(relativeHeadingAngle);
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

      for (int i = 0; i <= MAX_PATH_LENGTH - 1; i++) {

        lastIntersectionMarkerId = pathLocation[result][i];
        if (pathLocation[result][i] == -1) {
          lastIntersectionMarkerId = pathLocation[result][i-1];
          break;
        }

      }
    }
  }
}

void updateIntersectionLocalization(int intersectionType) {
  // Try to identify the current intersection marker id

  bool valid = false;
  int x = lastIntersectionMarkerId;
  int smallestDistance = 1000;
  int smallestDistanceMarkerID = -1;
  for (int y = 0; y < INTERSECTION_MARKER_COUNT; y++) {
    int expectedType = (int)pgm_read_byte(&(intersection_graph[y][x][1]));

    if (expectedType > 0 && expectedType == intersectionType) {
      // The current intersection is y

      byte currentIntersectionX = pgm_read_byte(&(intersections[y].intersectionX));
      byte currentIntersectionY = pgm_read_byte(&(intersections[y].intersectionY));


      /*Serial.print("Last: ");
        Serial.print(pgm_read_byte(&(intersections[x].id)));
        Serial.print(", Current: ");
        Serial.print(pgm_read_byte(&(intersections[y].id)));
        Serial.print(", Angle: ");
        Serial.println(angle);
      */
      // TODO: Perform realigmnent (angle and location) based on current location
      // TODO: Check car location and compare with current intersection location. If value is far away, this is likely not the correct intersection and should be skipped.

      double robotToInterectionDistance = sqrt(pow((absoluteLocationX) - (currentIntersectionX), 2) + pow((absoluteLocationY) - (currentIntersectionY), 2));
      if (robotToInterectionDistance < smallestDistance) {
        smallestDistance = robotToInterectionDistance;
        smallestDistanceMarkerID = y;
      }

    }
  }


  if (smallestDistanceMarkerID == -1) {
    // TODO: Couldn't figure out location..
    Serial.println("ERROR");
  }
  else {

    //angle and position correction
    // Calculate expected heading angle between the last and current intersection
    byte lastIntersectionX = pgm_read_byte(&(intersections[x].intersectionX));
    byte lastIntersectionY = pgm_read_byte(&(intersections[x].intersectionY));

    byte currentIntersectionX = pgm_read_byte(&(intersections[smallestDistanceMarkerID].intersectionX));
    byte currentIntersectionY = pgm_read_byte(&(intersections[smallestDistanceMarkerID].intersectionY));

    double xDiff = currentIntersectionX - lastIntersectionX;
    double yDiff = currentIntersectionY - lastIntersectionY;

    // TODO: This is in degrees for debugging
    double angle = atan2(yDiff, xDiff);

    absoluteHeadingAngle = angle;
    absoluteLocationX = currentIntersectionX;
    absoluteLocationY = currentIntersectionY;


    lastIntersectionMarkerId = smallestDistanceMarkerID;
  }
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
          break;
        }

      }

      return currentPath;
    }
  }
  return -1;
}






