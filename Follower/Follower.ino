
#include "Localization.h"
#include "Checkpoint.h"
#include "IntersectionDetection.h"
#include "LineFollower.h"
#include <LinkedList.h>
#include <math.h>
#include "Constants.h"
#include "Debug.h"
#include "Mapping.h"
#include "Pathfinder.h"
#include "LineFollower.h"
#include "Checkpoint.h"
#include "IntersectionDetection.h"
#include "Localization.h"

/*************Lane following PID Variables and Functions***********/
float Kp = 1.2; // 0.6; //0.3;
float Ki = 0.00;
float Kd = 500; //332; //450; //100;

float DERIVATIVE_SPEED_ADJUST = 150;
void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN1_LEFT_MOTOR, OUTPUT);
  pinMode(AIN2_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN2_LEFT_MOTOR, OUTPUT);
  pinMode(IR_DETECTOR, INPUT);
  pinMode(ANTENNA_LED, OUTPUT);
  pinMode(WALL_DISTANCE_SENSOR, INPUT);
  pinMode(12, INPUT);
  lastError = 0;
  averageMotorSpeed = 75;
  Serial.begin(9600);
  stallPWM = 65;
  previousTime = millis();

  //MoveRight();


  //SetMapElement(0, 0, true);

  //Serial.println(AccessMapElement(36, 4));
  //Serial.println(AccessMapElement(20, 4));

  //Pathfinder pathfinder;
  //PathfinderResult path = pathfinder.FindPath(Position(47, 19), Position(47, 18));

  //Serial.println("Values:");
  //for (int i = 0; i < path.size; i++) {
  //	Serial.println(path.path[i]->y);
  //}

  ////  Expected output:
  ////  19
  ////  18
  ////  17
  ////  16
  ////  15
  ////  14
  ////  13
  ////  12
  ////  11
  ////  10

  //Serial.print("Path Size: ");
  //Serial.println(path.size);

  initializeEncoders();
  interrupts();

  /*ProcessDetectedIntersection(INTERSECTION_TYPE_T);
  ProcessDetectedIntersection(INTERSECTION_TYPE_LEFTTURN);
  ProcessDetectedIntersection(INTERSECTION_TYPE_CROSS);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_T);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TLEFT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TLEFT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);
  ProcessDetectedIntersection(INTERSECTION_TYPE_LEFTTURN);
  ProcessDetectedIntersection(INTERSECTION_TYPE_CROSS);
  ProcessDetectedIntersection(INTERSECTION_TYPE_LEFTTURN);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);
  ProcessDetectedIntersection(INTERSECTION_TYPE_CROSS);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);*/

  /*Serial.print("Intersection: ");
  Serial.print(pgm_read_byte(&(intersections[lastIntersectionMarkerId].id)));
  Serial.print(", Marker: ");
  Serial.println(lastIntersectionMarkerId);*/
  // Expected: 17

  delay(20000);
}

void loop() {

  delay(10);
  long currentTime;

  // TODO: Debugging code
  //MotorSpeeds newMotorSpeeds;
  //newMotorSpeeds.left = 90;
  //newMotorSpeeds.right = 90;

  //if (newMotorSpeeds.left >= 0) {
  //	analogWrite(BIN2_LEFT_MOTOR, 0);
  //	analogWrite(BIN1_LEFT_MOTOR, newMotorSpeeds.left);//drives left motor forward
  //}
  //else {
  //	analogWrite(BIN1_LEFT_MOTOR, 0);
  //	analogWrite(BIN2_LEFT_MOTOR, -newMotorSpeeds.left);//drives left motor reverse
  //}

  //if (newMotorSpeeds.right >= 0) {
  //	analogWrite(AIN2_RIGHT_MOTOR, 0);
  //	analogWrite(AIN1_RIGHT_MOTOR, newMotorSpeeds.right);//drives right motor forward
  //}
  //else {
  //	analogWrite(AIN1_RIGHT_MOTOR, 0);
  //	analogWrite(AIN2_RIGHT_MOTOR, -newMotorSpeeds.right);//drives right motor reverse
  //}

  currentTime = millis();
  checkPointHandle(currentTime);
  updateFollowerState();
  ReadIntersectionSensors(currentTime);
  followLaneAnalog(currentTime);
  
  updateRelativeLocation();

  publishEncoderData(leftMotorCount, rightMotorCount);
  publishLocalizationData(currentTime);

  while (Serial.available() > 0) {
    int command = Serial.read();
    processDebugCommand(command);
  }
}
