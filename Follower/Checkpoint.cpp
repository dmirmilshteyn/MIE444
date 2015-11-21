#include "Checkpoint.h"

bool checkpointFound();
bool checkpointEventActive = false;
int checkpointTime;
int ledBlinkCount = 0;
bool ledOn = false;
int numCheckpointsFound = 0;
struct storedCheckpoint storedCheckpoint[CHECKPOINTS_TOTAL];

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


bool checkpointFound() {
  double checkpointDistance;
  for (int i = 0; i < numCheckpointsFound; i++) {
    checkpointDistance = sqrt(pow((relativeLocationX * MAP_RESOLUTION) - (storedCheckpoint[i].x * MAP_RESOLUTION), 2) + pow((relativeLocationY * MAP_RESOLUTION) - (storedCheckpoint[i].y * MAP_RESOLUTION), 2));
    if (checkpointDistance < 0.4) {
      return true;
    }
  }
  return false;
}
void checkPointHandle(int currentTime) {
  //what the robot will do when it reaches the checkponint
  if (numCheckpointsFound < CHECKPOINTS_TOTAL && !digitalRead(IR_DETECTOR)) {
    if (!checkpointFound) {
      //set current location of checkpoint as found
      storedCheckpoint[numCheckpointsFound].x = relativeLocationX;  //sets found checkpoint coordinates to current robot position
      storedCheckpoint[numCheckpointsFound].y = relativeLocationY;

      numCheckpointsFound++;
      checkpointTime = currentTime;
      checkpointEventActive = true;
    }
  }
  //following may not work if loop takes longer than 100ms
  if (checkpointEventActive && currentTime < (checkpointTime + CHECKPOINT_REACTION_DURATION)) {

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
  else {
    if (ledOn) {
      disableLED();
    }
    checkpointEventActive = false;
    //Serial.println("DONE");
  }
}
