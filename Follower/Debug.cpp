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
	case DEBUG_OUTPUT_STATE:
		print("PID Constants - Kp: ");
		Serial.print(Kp);
		print(", Ki: ");
		Serial.print(Ki);
		print(", Kd: ");
		Serial.println(Kd);
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
	}
}