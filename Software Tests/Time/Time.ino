void setup()
{
  unsigned long StartTime = micros();
}
void loop()
{
  unsigned long CurrentTime = micros();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  Serial.println(ElapsedTime);
}
