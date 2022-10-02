#include <Arduino.h>
#include <MeAuriga.h>
MeEncoderOnBoard RightMotor(SLOT1);
MeEncoderOnBoard LeftMotor(SLOT2);
double mapSpeed(double percent) {
  return percent / 100.0 * 255;
}
void setSpeed(double left, double right) {
  LeftMotor.setMotorPwm(left * 255);
  RightMotor.setMotorPwm(-right * 255);
}
void setSpeed(double val, bool left) {
  if (left) LeftMotor.setMotorPwm(val * 255);
  if (left) RightMotor.setMotorPwm(-val * 255);
}
void setup() {
  Serial.begin(9600);
}
void loop() {
  setSpeed(1.0, 1.0);
  delay(3000);
  setSpeed(1.0, false);
  delay(1000);
}