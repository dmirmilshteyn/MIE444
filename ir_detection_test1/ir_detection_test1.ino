void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(analogRead(4));
  Serial.print("Digital: ");
  Serial.print(digitalRead(2));
  Serial.print("  Analog: ");
  Serial.println(analogRead(4));

  //delay(100);
}
