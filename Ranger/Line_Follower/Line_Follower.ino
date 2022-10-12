#include <Arduino.h>
#include <MeAuriga.h>
int LINE_PIN = 9;
MeLineFollower lineFollower(LINE_PIN);
const int LW_RW = S1_OUT_S2_OUT;
const int LB_RW = S1_IN_S2_OUT;
const int LW_RB = S1_OUT_S2_IN;
const int LB_RB = S1_IN_S2_IN;
void setup() {
  Serial.begin(9600);
}
void loop() {
  uint8_t line_value = lineFollower.readSensors();
  switch (line_value) {
    case LB_RB:  // both sensors see black
      Serial.println("Both on black !");
      break;
    case LW_RB:  // S1 sees white , S2 sees black
      Serial.println("turning right!");
      break;
    case LB_RW:  // S1 sees black , S2 sees white
      Serial.println("turning left!");
      break;
    case LW_RW:  // both sensors see white
      Serial.println("Going Forward!");
      break;
  }
}