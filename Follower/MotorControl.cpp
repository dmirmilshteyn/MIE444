#include "Arduino.h"
#include "Constants.h"

void MoveLeft() {
  analogWrite(AIN2, 0);
  analogWrite(BIN2, 0);
  analogWrite(AIN1, 127);
  analogWrite(BIN1, 0);
}

void MoveRight() {
  analogWrite(AIN2, 0);
  analogWrite(BIN2, 0);
  analogWrite(AIN1, 0);
  analogWrite(BIN1, 127);
}

void MoveForward() {
  analogWrite(AIN2, 0);
  analogWrite(BIN2, 0);
  analogWrite(AIN1, 127);
  analogWrite(BIN1, 127);
}

