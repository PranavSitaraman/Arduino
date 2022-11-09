#include "global.h"
#include "util.h"
MePort_Sig mePort[17] = { { NC, NC }, { 5, 4 }, { 3, 2 }, { 7, 6 }, { 9, 8 }, { 16, 17 }, { A10, A15 }, { A9, A14 }, { A8, A13 }, { A7, A12 }, { A6, A11 }, { NC, A2 }, { NC, A3 }, { NC, A0 }, { NC, A1 }, { NC, NC }, { NC, NC } };
Encoder_port_type encoder_Port[6] = { { NC, NC, NC, NC, NC }, { 19, 42, 11, 49, 48 }, { 18, 43, 10, 47, 46 }, { NC, NC, NC, NC, NC }, { NC, NC, NC, NC, NC } };
MeEncoderOnBoard RightMotor(SLOT1);
MeEncoderOnBoard LeftMotor(SLOT2);
MeLineFollower lineFollower(LINE_PIN);
MeGyro gyro(0, gyroPin);
MeUltrasonicSensor usSensor(ULTRASONIC_PIN);
void setup() {
  Serial.begin(9600);
  Wire.begin();
  gyro.begin();
  initPWM8kHz();
  attachInterrupt(LeftMotor.getIntNum(), isr_LeftMotorEncoder, RISING);
  attachInterrupt(RightMotor.getIntNum(), isr_RightMotorEncoder, RISING);
  resetMotors();
}
void loop() {
  while (usSensor.distanceCm() > 6) followLine();
  irAvoid(1, 0.5);
}