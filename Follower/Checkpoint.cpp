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


bool checkpointFound() {
  double checkpointDistance;
  bool condition = false;
  for (int i = 0; i < numCheckpointsFound; i++) {
    checkpointDistance = sqrt(pow((absoluteLocationXMeters) - (storedCheckpoint[i].x * MAP_RESOLUTION), 2) + pow((absoluteLocationYMeters) - (storedCheckpoint[i].y * MAP_RESOLUTION), 2));
    if (checkpointDistance < 0.5) {
      condition = true;
    }
  }
//  Serial.print(absoluteLocationXMeters);
//  Serial.print("  ");
//  Serial.print(absoluteLocationYMeters);
//  Serial.print("  ");
//  Serial.println(condition);
  return condition;
}
void checkPointHandle(long currentTime) {
//    Serial.print(digitalRead(IR_DETECTOR));
//    Serial.print("  ");
//    Serial.println(analogRead(5));
//****************need large potentiometer (>200K)**********


  //what the robot will do when it reaches the checkponint
  if (numCheckpointsFound < CHECKPOINTS_TOTAL && digitalRead(IR_DETECTOR) == 0 && checkpointEventActive == false) {
    //    Serial.println("test");
    if (checkpointFound() == false) {
      //      Serial.println("test2");
      //set current location of checkpoint as found
      storedCheckpoint[numCheckpointsFound].x = absoluteLocationX;  //sets found checkpoint coordinates to current robot position
      storedCheckpoint[numCheckpointsFound].y = absoluteLocationY;

      numCheckpointsFound++;
      checkpointTime = currentTime;
      checkpointEventActive = true;
      ledBlinkCount = 1;

    }
  }
  //following may not work if loop takes longer than 100ms
  if (checkpointEventActive && currentTime < (checkpointTime + CHECKPOINT_REACTION_DURATION)) {

    int LEDonMillis = checkpointTime + ledBlinkCount * 600;
    int LEDoffMillis = checkpointTime + (ledBlinkCount * 600) + 300;
    //    Serial.print(roundToHundreds(currentTime));
    //    Serial.print("  ");
    //    Serial.print(roundToHundreds(LEDonMillis));
    //    Serial.print("  ");
    //    Serial.println(roundToHundreds(LEDoffMillis));
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
    //Serial.println("DONE");
  }
}
