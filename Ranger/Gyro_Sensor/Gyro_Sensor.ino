#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
int gyroPin = 0x69;
MeGyro gyro(0, gyroPin);
MeEncoderOnBoard RightMotor(SLOT1);
MeEncoderOnBoard LeftMotor(SLOT2);
void setup() {
  Serial.begin(9600);
  Wire.begin();
  gyro.begin();
}
void loop() {
  RightMotor.loop();
  LeftMotor.loop();
  int x = gyro.getAngle(2);  //Z –using the internal gyro sensor.
  if (x < 90) {
    RightMotor.setMotorPwm(125);
    LeftMotor.setMotorPwm(125);  // left motor is mounted opposite dir vs RightMotor
  } else {
    RightMotor.setMotorPwm(0);
    LeftMotor.setMotorPwm(0);  // left motor is mounted opposite dir vs RightMotor
  }
  delay(50);
}