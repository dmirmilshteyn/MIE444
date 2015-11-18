#include "Debug.h"
#include "Constants.h"

inline void print(const char* message) {
	Serial.print(message);
}

inline void print(float value) {
	Serial.print(value);
}

inline void println() {
	Serial.println();
}

inline void println(const char* message) {
	Serial.println(message);
}

inline void println(float value) {
	Serial.print(value);
}

int readDebugCommand() {
	if (Serial.available() > 0) {
		return Serial.read();
	}
}

void publishLaneFollowingData(MotorSpeeds motorSpeeds, float currentError, float integral, float derivative, float controller) {
	print("!?");
	print(currentError);
	print("|");
	print(integral);
	print("|");
	print(derivative);
	print("|");
	print(controller);
	print("|");
	print(motorSpeeds.left);
	print("|");
	print(motorSpeeds.right);
	println();
}

void publishIntersectionDetectionData(int frontSensor, int leftSensor, int rightSensor) {
	print("!#");
	print(frontSensor);
	print("|");
	print(leftSensor);
	print("|");
	print(rightSensor);
	println();
}

void processDebugCommand(int command) {
	switch (command) {
	case DEBUG_REQUEST_SYNC:
		Serial.read();
		Serial.print("!!");
		Serial.print(Kp);
		Serial.print("|");
		Serial.print(Ki);
		Serial.print("|");
		Serial.print(Kd);
		Serial.print("|");
		Serial.print(DERIVATIVE_SPEED_ADJUST);
		Serial.print("|");
		Serial.print(averageMotorSpeed);
		Serial.println();
		break;
	case DEBUG_OUTPUT_STATE:
		Serial.read();
		print("PID Constants - Kp: ");
		print(Kp);
		print(", Ki: ");
		print(Ki);
		print(", Kd: ");
		println(Kd);
		break;
	case DEBUG_SET_P:
		Kp = Serial.parseFloat();
		Serial.read();
		print("Kp = ");
		println(Kp);
		break;
	case DEBUG_SET_I:
		Ki = Serial.parseFloat();
		Serial.read();
		print("Ki = ");
		println(Ki);
		break;
	case DEBUG_SET_D:
		Kd = Serial.parseFloat();
		Serial.read();
		print("Kd = ");
		println(Kd);
		break;
	case DEBUG_SPEED_ADJUST:
		DERIVATIVE_SPEED_ADJUST = Serial.parseFloat();
		Serial.read();
		print("Speed Adjust = ");
		println(DERIVATIVE_SPEED_ADJUST);
		break;
	case DEBUG_AVERAGE_SPEED:
		averageMotorSpeed = Serial.parseInt();
		Serial.read();
		print("Average Motor Speed = ");
		println(averageMotorSpeed);
		break;
	}
}