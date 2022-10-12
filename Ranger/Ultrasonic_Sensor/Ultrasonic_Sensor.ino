#include <Arduino.h>
#include <MeAuriga.h>
int ULTRASONIC_PIN = 10;
MeBuzzer buzzer;
MeUltrasonicSensor usSensor(ULTRASONIC_PIN);
void setup() {
  Serial.begin(9600);  // for serial monitor display
  buzzer.setpin(45);   // setup up the buzzer
}
void loop() {
  if (usSensor.distanceCm() < 12) {
    buzzer.tone(262, 0.25 * 1000);
    delay(.25);
  }
}