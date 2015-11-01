#include "Constants.h"
#include "MotorController.h"
#include "Mapping.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);

  Serial.begin(115200);

  InitializeMapping();

  MoveRight();


  SetMapElement(0, 0, true);
  
  Serial.println(AccessMapElement(1, 0));
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
