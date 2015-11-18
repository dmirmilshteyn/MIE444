
#include "Checkpoint.h"
#include "IntersectionDetection.h"
#include "LineFollower.h"
#include <LinkedList.h>
#include <math.h>
#include "Constants.h"
#include "Debug.h"
#include "MotorController.h"
#include "Mapping.h"
#include "Pathfinder.h"
#include "LineFollower.h"
#include "Checkpoint.h"
#include "IntersectionDetection.h"

/*************Lane following PID Variables and Functions***********/
float Kp = 0.6; //0.3;
float Ki = 0.00;
float Kd = 450; //100;

float DERIVATIVE_SPEED_ADJUST = 0;
void setup() {
	// put your setup code here, to run once:
	pinMode(AIN1_RIGHT_MOTOR, OUTPUT);
	pinMode(BIN1_LEFT_MOTOR, OUTPUT);
	pinMode(AIN2_RIGHT_MOTOR, OUTPUT);
	pinMode(BIN2_LEFT_MOTOR, OUTPUT);
	pinMode(IR_DETECTOR, INPUT);
	pinMode(ANTENNA_LED, OUTPUT);
	lastError = 0;
	averageMotorSpeed = 90;
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
}

void loop() {
	//Serial.print("Digital: ");
	//Serial.println(digitalRead(IR_DETECTOR));
	delay(10);
	int currentTime;


	currentTime = millis();
	//checkPointHandle(currentTime);
	//followLaneAnalog(currentTime);
	ReadIntersectionSensors();

	while (Serial.available() > 0) {
		int command = Serial.read();
		processDebugCommand(command);
	}
}