#include "Debug.h"
#include "Constants.h"

void print(const char* message) {
	Serial.print(message);
}

void println(const char* message) {
	Serial.println(message);
}

int readDebugCommand() {
	while (Serial.available() > 0) {
		return Serial.read();
	}
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
		Serial.print(Kp);
		print(", Ki: ");
		Serial.print(Ki);
		print(", Kd: ");
		Serial.println(Kd);
		break;
	case DEBUG_SET_P:
		Kp = Serial.parseFloat();
		Serial.read();
		print("Kp = ");
		Serial.println(Kp);
		break;
	case DEBUG_SET_I:
		Ki = Serial.parseFloat();
		Serial.read();
		print("Ki = ");
		Serial.println(Ki);
		break;
	case DEBUG_SET_D:
		Kd = Serial.parseFloat();
		Serial.read();
		print("Kd = ");
		Serial.println(Kd);
		break;
	case DEBUG_SPEED_ADJUST:
		DERIVATIVE_SPEED_ADJUST = Serial.parseFloat();
		Serial.read();
		print("Speed Adjust = ");
		Serial.println(DERIVATIVE_SPEED_ADJUST);
		break;
	case DEBUG_AVERAGE_SPEED:
		averageMotorSpeed = Serial.parseInt();
		Serial.read();
		print("Average Motor Speed = ");
		Serial.println(averageMotorSpeed);
		break;
	}
}