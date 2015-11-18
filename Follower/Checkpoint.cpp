#include "Checkpoint.h"

bool checkPointFound = false;
bool checkPointComplete = false;
int checkpointTime;
int ledBlinkCount = 0;
bool ledOn = false;

void enableLED() {
	digitalWrite(ANTENNA_LED, HIGH);
	//Serial.println("ON");
	ledOn = true;
}
void disableLED() {
	digitalWrite(ANTENNA_LED, LOW);
	//Serial.println("OFF");
	ledOn = false;
}
void checkPointHandle(int currentTime) {
	//what the robot will do when it reaches the checkponint
	if (!digitalRead(IR_DETECTOR) && !checkPointFound) {
		checkPointFound = true;
		checkpointTime = currentTime;
	}
	//following may not work if loop takes longer than 100ms
	if (currentTime < (checkpointTime + CHECKPOINT_REACTION_DURATION) && checkPointFound && !checkPointComplete) {

		if (roundToHundreds(currentTime) == roundToHundreds(checkpointTime + ledBlinkCount * 600)) {
			if (!ledOn) {
				enableLED();
			}
		}

		else if (roundToHundreds(currentTime) == roundToHundreds(checkpointTime + (ledBlinkCount * 600) + 300)) {
			if (ledOn) {
				disableLED();
				ledBlinkCount++;
			}
		}

	}
	else if (ledOn) {
		disableLED();
		checkPointComplete = true;
		//Serial.println("DONE");
	}
}
