#include "Checkpoint.h"

bool checkpointFound();
bool checkpointEventActive = false;
long checkpointTime;
int ledBlinkCount;
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

//function checks if the current detected checkpoint has alredy been found (based on rover location on map)
bool checkpointFound() {
  double checkpointDistance;
  bool condition = false;
  for (int i = 0; i < numCheckpointsFound; i++) {
    checkpointDistance = sqrt(pow((absoluteLocationXMeters) - (storedCheckpoint[i].x * MAP_RESOLUTION), 2) + pow((absoluteLocationYMeters) - (storedCheckpoint[i].y * MAP_RESOLUTION), 2));
    if (checkpointDistance < 0.2) {
      condition = true;
    }
  }
  return condition;
}

//what the robot will do when it reaches the checkponint
void checkPointHandle(long currentTime) {
  if (numCheckpointsFound < CHECKPOINTS_TOTAL && digitalRead(IR_DETECTOR) == 0 && checkpointEventActive == false) {
    if (checkpointFound() == false) {
      storedCheckpoint[numCheckpointsFound].x = absoluteLocationX;  //sets found checkpoint coordinates to current robot position
      storedCheckpoint[numCheckpointsFound].y = absoluteLocationY;

      numCheckpointsFound++;
      checkpointTime = currentTime;
      checkpointEventActive = true;
      ledBlinkCount = 1;

    }
  }

  //Once checkpoint is detected, LED will keep blinking at 300ms intervals for CHECKPOINT_REACTION_DURATION miliseconds
  if (checkpointEventActive && currentTime < (checkpointTime + CHECKPOINT_REACTION_DURATION)) {

    int LEDonMillis = checkpointTime + ledBlinkCount * 600;
    int LEDoffMillis = checkpointTime + (ledBlinkCount * 600) + 300;
    if (roundToHundreds(currentTime) == roundToHundreds(LEDonMillis)) {
      if (!ledOn) {
        enableLED();
      }
    }

    else if (roundToHundreds(currentTime) == roundToHundreds(LEDoffMillis)) {
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
  }
}
