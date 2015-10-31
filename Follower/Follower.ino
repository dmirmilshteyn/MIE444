#include "Constants.h"
#include "MotorController.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);

  MoveRight();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
