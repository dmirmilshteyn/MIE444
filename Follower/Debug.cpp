#include "Debug.h"
#include "Constants.h"

inline void print(const char* message) {
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print(message);
#endif
}

inline void print(float value) {
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print(value);
#endif
}

inline void print(double value) {
#ifdef PRINT_DEBUG_MESSAGES
	Serial.print(value);
#endif
}

inline void print(long value) {
#ifdef PRINT_DEBUG_MESSAGES
	Serial.print(value);
#endif
}

inline void print(int value) {
#ifdef PRINT_DEBUG_MESSAGES
	Serial.print(value);
#endif
}

inline void println() {
#ifdef PRINT_DEBUG_MESSAGES
  Serial.println();
#endif
}

inline void println(const char* message) {
#ifdef PRINT_DEBUG_MESSAGES
  Serial.println(message);
#endif
}

inline void println(float value) {
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print(value);
#endif
}

int readDebugCommand() {
  if (Serial.available() > 0) {
    return Serial.read();
  }
}

long lastLocalizationPublishTime = 0;
long lastIntersectionPublishTime = 0;

void publishLaneFollowingData(MotorSpeeds motorSpeeds, float currentError, float integral, float derivative, float controller, float leftLineSensor, float rightLineSensor) {
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
  print("|");
  print(leftLineSensor);
  print("|");
  print(rightLineSensor);
  println();
}

void publishIntersectionDetectionData(long currentTime, int frontSensor, int leftSensor, int rightSensor, int detectedIntersection) {
//	if (currentTime > lastIntersectionPublishTime + 1000) {
//		lastIntersectionPublishTime = currentTime;
//
//		Serial.print("!#");
//		Serial.print(frontSensor);
//		Serial.print("|");
//		Serial.print(leftSensor);
//		Serial.print("|");
//		Serial.print(rightSensor);
//		Serial.print("|");
//		Serial.print(detectedIntersection);
//		Serial.println();
//	}
}

void publishEncoderData(long leftMotorCount, long rightMotorCount) {
  print("!$");
  print(leftMotorCount);
  print("|");
  print(rightMotorCount);
  println();
}

void publishMap() {
  print("!*");
  print(MAP_TILES_WIDTH);
  print("|");
  print(MAP_TILES_HEIGHT);
  /*for (int x = 0; x < MAP_TILES_WIDTH; x++) {
  	for (int y = 0; y < MAP_TILES_HEIGHT; y++) {
  		bool blocked = AccessMapElement(x, y);

  		print(blocked);
  		print("|");
  	}
    }*/
  println();
}
void publishLocalizationData(long currentTime) {
//	if (currentTime > lastLocalizationPublishTime + 1000) {
//		lastLocalizationPublishTime = currentTime;
//		Serial.print("!&");
//		Serial.print(relativeLocationXMeters);
//		Serial.print("|");
//		Serial.print(relativeLocationYMeters);
//		Serial.print("|");
//
//		int currentIntersection = (int)pgm_read_byte(&(intersections[lastIntersectionMarkerId].id));
//		Serial.print(currentIntersection);
//		Serial.println();
//	}
}

void processDebugCommand(int command) {
  switch (command) {
    case DEBUG_SYNC_MAP:
      Serial.read();
      //publishMap();
      break;
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
	  Serial.print("|");
	  Serial.print(stallPWM);
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
      /*averageMotorSpeed = */Serial.parseInt();
      Serial.read();
      print("Average Motor Speed = ");
      println(averageMotorSpeed);
      break;
    case DEBUG_STALL_PWM:
      stallPWM = Serial.parseInt();
      Serial.read();
      print("Stall PWM = ");
      println(stallPWM);
      break;
  }
}
