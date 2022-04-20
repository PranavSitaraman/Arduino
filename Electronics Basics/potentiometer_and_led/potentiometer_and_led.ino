#include<Arduino.h>
void setup() {
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(A1);
  analogWrite(6, map(val, 0, 1023, 0, 255));
  Serial.println(val);
}
