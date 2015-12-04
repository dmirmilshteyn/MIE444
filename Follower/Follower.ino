
#include "Planner.h"
#include "IntersectionPathfinder.h"
#include "Localization.h"
#include "Checkpoint.h"
#include "IntersectionDetection.h"
#include "LineFollower.h"
#include <math.h>
#include "Constants.h"
#include "Debug.h"
#include "LineFollower.h"
#include "Checkpoint.h"
#include "IntersectionPathfinder.h"
#include "IntersectionDetection.h"
#include "Localization.h"

/*************Lane following PID Variables and Functions***********/
float Kp = 1; // 0.6; //0.3;
float Ki = 0;//0.0009;//0.0015;
float Kd = 600;//400; //332; //450; //100;

long tempTime = 0;
float DERIVATIVE_SPEED_ADJUST = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN1_LEFT_MOTOR, OUTPUT);
  pinMode(AIN2_RIGHT_MOTOR, OUTPUT);
  pinMode(BIN2_LEFT_MOTOR, OUTPUT);
  pinMode(IR_DETECTOR, INPUT);
  pinMode(ANTENNA_LED, OUTPUT);
  pinMode(WALL_DISTANCE_SENSOR, INPUT);
  pinMode(WALL_COLOUR_SENSOR, INPUT);
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

  /*IntersectionPathfinder pathfinder(0.0);
  auto path = pathfinder.FindPath(14, 15);

  Serial.println("Values:");
  for (int i = 0; i < path.size; i++) {
	  Serial.print("Intersection: ");
  	  Serial.print(pgm_read_byte(&(intersections[path.path[i]].id)));
	  Serial.print(", Target Angle: ");
	  Serial.println(path.pathTurns[i]);
  }*/

  initializeEncoders();
  interrupts();

  delay(15000);

  /*digitalWrite(ANTENNA_LED, HIGH);

  ProcessDetectedIntersection(INTERSECTION_TYPE_T);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);*/

  //ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  /*ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);*/

  //ProcessDetectedIntersection(INTERSECTION_TYPE_T);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_LEFTTURN);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_CROSS);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_T);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TLEFT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TLEFT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_LEFTTURN);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_CROSS);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_LEFTTURN);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_CROSS);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_TRIGHT);
  //ProcessDetectedIntersection(INTERSECTION_TYPE_RIGHTTURN);

  //Serial.print("Intersection: ");
  //Serial.print(pgm_read_byte(&(intersections[lastIntersectionMarkerId].id)));
  //Serial.print(", Marker: ");
  //Serial.println(lastIntersectionMarkerId);
  // Expected: 17

 
}

void loop() {

  delay(10);
  unsigned long currentTime;

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
  updateFollowerState(currentTime);
  ReadIntersectionSensors(currentTime);
  followLaneAnalog(currentTime);
  //wallDetection(currentTime);
  
  updateRelativeLocation();

  publishEncoderData(leftMotorCount, rightMotorCount);
  publishLocalizationData(currentTime);

 

  /*long loopTime = millis() - currentTime;

  Serial.println(loopTime);*/

  while (Serial.available() > 0) {
    int command = Serial.read();
    processDebugCommand(command);
  }
}
