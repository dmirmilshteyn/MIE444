#include "Arduino.h"
#include "Constants.h"

void MoveLeft() {
  analogWrite(AIN2_RIGHT_MOTOR, 0);
  analogWrite(BIN2_LEFT_MOTOR, 0);
  analogWrite(AIN1_RIGHT_MOTOR, 127);
  analogWrite(BIN1_LEFT_MOTOR, 0);
}

void MoveRight() {
  analogWrite(AIN2_RIGHT_MOTOR, 0);
  analogWrite(BIN2_LEFT_MOTOR, 0);
  analogWrite(AIN1_RIGHT_MOTOR, 0);
  analogWrite(BIN1_LEFT_MOTOR, 127);
}

void MoveForward() {
  analogWrite(AIN2_RIGHT_MOTOR, 0);
  analogWrite(BIN2_LEFT_MOTOR, 0);
  analogWrite(AIN1_RIGHT_MOTOR, 127);
  analogWrite(BIN1_LEFT_MOTOR, 127);
}

