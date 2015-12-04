#include "IntersectionDetection.h"

int detectedIntersection = INTERSECTION_TYPE_NONE;
int detectedMarkerId = -1;
long lastIntersectionEncoderTick = 0;
int currentFront = 0;
int currentLeft = 0;
int currentRight = 0;

int previousTestIntersection = INTERSECTION_TYPE_NONE;
int currentTestIntersection = INTERSECTION_TYPE_NONE;

long lastLeftEncoder = 0;
long lastRightEncoder = 0;

long lastIntersectionDetectionLeftEncoder = 0;
long lastIntersectionDetectionRightEncoder = 0;

float deadEndAngles[DEAD_END_MAX][2] = {
  {3, 0},
  {25, -M_PI / 2},
  {21, -M_PI / 2},
  {14, M_PI / 2},
  {9, -M_PI / 2},
  {39, -M_PI / 3}
};



double lastTick = 0;

void ReadIntersectionSensors(long tick) {
  int sensorA = analogRead(LINE_SENSOR_1);
  int sensorB = analogRead(LINE_SENSOR_2);
  int sensorC = analogRead(LINE_SENSOR_3);

  publishIntersectionDetectionData(tick, sensorA, sensorB, sensorC, detectedIntersection);

  currentFront = sensorA;
  currentLeft = sensorB;
  currentRight = sensorC;

  if (followerState == FOLLOWER_STATE_ONLINE && !isRealigning) {
    if (leftMotorCount > lastIntersectionDetectionLeftEncoder + 600 / 4 &&
        rightMotorCount > lastIntersectionDetectionRightEncoder + 600 / 4) {
      IdentifyIntersection(tick, sensorA, sensorB, sensorC, leftMotorCount, rightMotorCount);

      if (detectedIntersection != INTERSECTION_TYPE_NONE) {
        //        Serial.println("process: ");
        //        Serial.println(detectedIntersection);
        ProcessDetectedIntersection(detectedIntersection);
      }
    }
  }
}

void IdentifyIntersection(int tick, int frontSensor, int leftSensor, int rightSensor, long encoderLeft, long encoderRight) {
  currentTestIntersection = INTERSECTION_TYPE_NONE;

  if (IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT)) {
    currentTestIntersection = INTERSECTION_TYPE_LEFTTURN;
  }
  else if (IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
    currentTestIntersection = INTERSECTION_TYPE_RIGHTTURN;
  }

  if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) && IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT)) {
    currentTestIntersection = INTERSECTION_TYPE_TLEFT;
  }
  else if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) && IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
    currentTestIntersection = INTERSECTION_TYPE_TRIGHT;
  }

  if (IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT) && IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
    currentTestIntersection = INTERSECTION_TYPE_T;
  }

  if (IsSensorOnOrApproaching(SENSOR_LOCATION_FRONT) && IsSensorOnOrApproaching(SENSOR_LOCATION_LEFT) && IsSensorOnOrApproaching(SENSOR_LOCATION_RIGHT)) {
    currentTestIntersection = INTERSECTION_TYPE_CROSS;
  }

  //if (abs(encoderLeft - lastLeftEncoder) > 225) {
  //  detectedIntersection = currentTestIntersection;
  //  verifyDetectedIntersection();
  //}
  //else {
  double currentTick = (encoderLeft + encoderRight) / 2;
  if (currentTestIntersection != previousTestIntersection) {
    detectedIntersection = INTERSECTION_TYPE_NONE;
    previousTestIntersection = currentTestIntersection;
    lastTick = currentTick;
  }
  else {
    double testTick = lastTick + GetEncoderDistanceTicks();
    if (currentTick > testTick) {
      detectedIntersection = currentTestIntersection;
      verifyDetectedIntersection();
      lastTick = currentTick;
    }
  }
  //}

  //lastLeftEncoder = encoderLeft;
  //lastRightEncoder = encoderRight;
}

void verifyDetectedIntersection() {

  //  Serial.print("intersection: ");
  //  Serial.println(detectedIntersection);
  if (detectedIntersection != INTERSECTION_TYPE_NONE && currentPath > -1) {

    //    Serial.print(" detected");
    //    Serial.println(detectedIntersection);
    // TODO: verify that correct intersection was detected with encoders
    // get previous intersection marker ID x and y values. get neighboring intersection marker x and y values and compare with robot location.
    // the smallest radius is the matched intersection.
    // might need to limit the radius if the robot detects an intersection where there isnt one.

    int x = lastIntersectionMarkerId;
    byte lastIntersectionX = pgm_read_byte(&(intersections[x].intersectionX));
    byte lastIntersectionY = pgm_read_byte(&(intersections[x].intersectionY));

    double smallestDistance = 1000;
    double smallestAngle = 1000;

    int smallestDistanceMarkerID = -1;
    byte deadEndCase = 0;

    double pow3 = ((double)absoluteLocationX) * MAP_RESOLUTION - ((double)lastIntersectionX) * MAP_RESOLUTION;
    double pow4 = ((double)absoluteLocationY) * MAP_RESOLUTION - ((double)lastIntersectionY) * MAP_RESOLUTION;
    double testDistance = sqrt(pow(pow3, 2) + pow(pow4, 2));
    double testMarkerId = -1;
    double testAngle = M_PI;


    switch (lastIntersectionMarkerId) {
      case 0:
      case 1:
      case 2:
        testMarkerId = 3;
        deadEndCase = 1;
        break;
      case 27:
      case 26:
        testMarkerId = 25;
        deadEndCase = 1;
        break;
      case 19:
      case 20:
        testMarkerId = 21;
        deadEndCase = 1;
        break;
      case 13:
      case 15:
        testMarkerId = 14;
        deadEndCase = 1;
        break;
      case 8:
      case 10:
        testMarkerId = 9;
        deadEndCase = 1;
        break;
      case 40:
      case 41:
        testMarkerId = 39;
        deadEndCase = 1;
        break;
    }

    for (int i = 0; i < DEAD_END_MAX; i++) {
      if (testMarkerId == (int)deadEndAngles[i][0]) {
        testAngle = deadEndAngles[i][1];
        break;
      }
    }

    double testAngleCompare = abs(normalise((absoluteHeadingAngle - testAngle), -M_PI, M_PI));

    long currentEncoderTick = (leftMotorCount + rightMotorCount) / 2;
    long encoderTickDiff = currentEncoderTick - lastIntersectionEncoderTick;
    double encoderTickDiffMetres = (double)encoderTickDiff / GEAR_RATIO / ENCODER_TEETH_COUNT * WHEEL_RADIUS * 2 * M_PI;
Serial.print(" encoderTickDiffMetres:");
Serial.println(encoderTickDiffMetres);
    if (testDistance < 0.2 && testAngleCompare < 20 * 180 / M_PI && deadEndCase == 1 && encoderTickDiffMetres > 0.25) {
      detectedMarkerId = testMarkerId;
      detectedIntersection = pgm_read_byte(&(intersections[detectedMarkerId].type));
      smallestAngle = 0;

      Serial.print(" lastS");
      Serial.print(lastIntersectionMarkerId);
      Serial.print(" checkS");
      Serial.print(detectedMarkerId);
      Serial.print("   ");

    }
    else {
      deadEndCase = 0;
    }
    // }
    if (deadEndCase == 0) {
      for (int i = 0; i < INTERSECTION_MARKER_COUNT; i++) {
        if ((int8_t)pgm_read_byte(&(intersection_graph[i][lastIntersectionMarkerId][0])) != -1) {
          byte neighborIntersectionX = pgm_read_byte(&(intersections[i].intersectionX));
          byte neighborIntersectionY = pgm_read_byte(&(intersections[i].intersectionY));
          double pow1 = ((double)absoluteLocationX) * MAP_RESOLUTION - ((double)neighborIntersectionX) * MAP_RESOLUTION;
          pow1 = pow(pow1, 2);
          double pow2 = ((double)absoluteLocationY) * MAP_RESOLUTION - ((double)neighborIntersectionY) * MAP_RESOLUTION;
          pow2 = pow(pow2, 2);
          double robotToIntersectionDistance = sqrt(pow1 + pow2);

          double xDiff = neighborIntersectionX - lastIntersectionX;
          double yDiff = neighborIntersectionY - lastIntersectionY;

          double neighborAngle = atan2(yDiff, xDiff);
          switch (i) {
            case 10:
              neighborAngle = -M_PI;
              break;
            case 11:
              neighborAngle = M_PI / 2;
              break;
            case 43:
              neighborAngle = -M_PI * 0.9;
              break;
            case 38:
              neighborAngle = M_PI / 4;
              break;
            case 7:
              neighborAngle = M_PI * 0.9;
              break;
            case 8:
              neighborAngle = 0;
              break;
          }


          double angleCompare = abs(normalise((absoluteHeadingAngle - neighborAngle), -M_PI, M_PI));


          if (robotToIntersectionDistance < smallestDistance) {
            //            Serial.print(" distance");
            //            Serial.println(robotToIntersectionDistance);
            //            Serial.print(" x:");
            //            Serial.print(absoluteLocationX);
            //            Serial.print(" y:");
            //            Serial.print(absoluteLocationY);
            //            Serial.print("     x:");
            //            Serial.print(neighborIntersectionX);
            //            Serial.print(" y:");
            //            Serial.print(neighborIntersectionY);
            //            Serial.print("     pow1:");
            //            Serial.print(pow1);
            //            Serial.print(" pow2:");
            //            Serial.println(pow2);


            smallestDistance = robotToIntersectionDistance;
            smallestAngle = angleCompare;
            smallestDistanceMarkerID = i;
            //            Serial.print(" angle");
            //            Serial.println(smallestAngle);

          }
        }
      }

      if (smallestDistance < 0.2 && smallestAngle < 20 * M_PI / 180) {
        detectedIntersection = pgm_read_byte(&(intersections[smallestDistanceMarkerID].type));
        detectedMarkerId = smallestDistanceMarkerID;
        //        Serial.print(" distance");
        //        Serial.println(smallestDistance);
        Serial.print(" last");
        Serial.print(lastIntersectionMarkerId);
        Serial.print(" check");
        Serial.print(detectedMarkerId);
        Serial.print("   ");
      }
      else {
        //        Serial.println("check didnt work");
        detectedIntersection = INTERSECTION_TYPE_NONE;
      }
    }
  }

  //  Serial.print("Corrected: ");
  //  Serial.println(detectedIntersection);
}

int GetCurrentSensorValue(int sensorLocation) {
  switch (sensorLocation) {
    case SENSOR_LOCATION_FRONT:
      return currentFront;
    case SENSOR_LOCATION_LEFT:
      return currentLeft;
    case SENSOR_LOCATION_RIGHT:
      return currentRight;
  }
}

int GetSensorOnThreshold(int sensorLocation) {
  switch (sensorLocation) {
    case SENSOR_LOCATION_FRONT:
      return FRONT_ON_THRESHOLD;
    case SENSOR_LOCATION_LEFT:
      return LEFT_ON_THRESHOLD;
    case SENSOR_LOCATION_RIGHT:
      return RIGHT_ON_THRESHOLD;
  }
}

int GetSensorApproachingThreshold(int sensorLocation) {
  switch (sensorLocation) {
    case SENSOR_LOCATION_FRONT:
      return FRONT_APPROACHING_THRESHOLD;
    case SENSOR_LOCATION_LEFT:
      return LEFT_APPROACHING_THRESHOLD;
    case SENSOR_LOCATION_RIGHT:
      return RIGHT_APPROACHING_THRESHOLD;
  }
}

bool IsSensorOn(int sensorLocation) {
  int onThreshold = GetSensorOnThreshold(sensorLocation);
  int value = GetCurrentSensorValue(sensorLocation);

  return (value >= onThreshold);
}

bool IsSensorApproaching(int sensorLocation) {
  int approachingThreshold = GetSensorApproachingThreshold(sensorLocation);
  int onThreshold = GetSensorOnThreshold(sensorLocation);
  int value = GetCurrentSensorValue(sensorLocation);

  return (value > approachingThreshold && value < onThreshold);
}

bool IsSensorOnOrApproaching(int sensorLocation) {
  return (IsSensorApproaching(sensorLocation) || IsSensorOn(sensorLocation));
}
