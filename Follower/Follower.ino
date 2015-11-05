
#include <math.h>
#include "Constants.h"
#include "MotorController.h"
#include "Mapping.h"
bool checkPointFound = false;
bool checkPointComplete = false;
void checkPointHandle(int currentTime);
void enableLED();
void disableLED();
int checkpointTime;
int ledBlinkCount = 0;
bool ledOn = false;
int roundToHundreds(int num);


void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(IR_DETECTOR, INPUT);
  pinMode(ANTENNA_LED, OUTPUT);


  Serial.begin(115200);

  //MoveRight();


  //SetMapElement(0, 0, true);

  //Serial.println(AccessMapElement(1, 0));
}

void loop() {
  //put your main code here, to run repeatedly:
  //Serial.print("Digital: ");
  //Serial.println(digitalRead(IR_DETECTOR));
  int currentTime = millis();
  checkPointHandle(currentTime);
}

void enableLED() {
  digitalWrite(ANTENNA_LED, HIGH);
  Serial.println("ON");
  ledOn = true;

}
void disableLED() {
  digitalWrite(ANTENNA_LED, LOW);
  Serial.println("OFF");
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
    checkPointComplete=true;
    Serial.println("DONE");
  }
}

int roundToHundreds(int num) {
  return round(num / 100) * 100;

}





