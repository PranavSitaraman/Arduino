volatile int encoder = 1;
int dist = 360;
void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(3), update, RISING);
  digitalWrite(12, 0);
}

void loop() {
  while (encoder < dist)
  {
    analogWrite(3, constrain(int(dist * 75.0/(encoder)), 0, 250));
  }
  analogWrite(3, 0);
  delay(1000);
  encoder = 1;
}
void update()
{
  encoder += 1;
}
