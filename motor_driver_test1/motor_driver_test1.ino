int AIN1=3;
int AIN2=5;
int BIN2=6;
int BIN1=9;
int outSpeed=40;
int increment = 5;
int forward=1;
int backward=0;

int direction1=forward;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop() {

  if(direction1==forward){
    analogWrite(AIN1, 127);
    analogWrite(BIN1, 127);
  
    delay(900);
    
    analogWrite(AIN1, 0);
    analogWrite(BIN1, 0);
    
    delay(700);
    direction1=backward;
  }
  else if(direction1==backward){

    analogWrite(AIN2, 127);
    analogWrite(BIN2, 127);

    delay(900);
    
    analogWrite(AIN2, 0);
    analogWrite(BIN2, 0);
    delay(700);
    direction1=forward;
  }

  
  
}
