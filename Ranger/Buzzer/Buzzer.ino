#include <Arduino.h>
#include <MeAuriga.h>
MeBuzzer buzzer;  // create the object name buzzer with type MeBuzzer
void setup() {
  buzzer.setpin(45);  // 45 is the assigned pin # for the internal buzzer device
}
void loop() {
  buzzer.tone(262, 0.25 * 1000);
  delay(1);
}