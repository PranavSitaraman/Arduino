#ifndef UTIL_CPP
#define UTIL_CPP
#include "global.h"
#include "util.h"
void setSpeed(double left, double right) {
  RightMotor.setMotorPwm((right * -255));
  LeftMotor.setMotorPwm((left * 255));
}
void setSpeed(double val, bool left) {
  if (left == 0) RightMotor.setMotorPwm((val * -255));
  else LeftMotor.setMotorPwm((val * 255));
}
void isr_LeftMotorEncoder(void) {
  if (digitalRead(LeftMotor.getPortB()) == 0) LeftMotor.pulsePosMinus();
  else LeftMotor.pulsePosPlus();
}
void isr_RightMotorEncoder(void) {
  if (digitalRead(RightMotor.getPortB()) == 0) RightMotor.pulsePosMinus();
  else RightMotor.pulsePosPlus();
}
void initPWM8kHz() {
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}
void move(double cm, double speed, uint8_t type) {
  if (type == MOVEMENT::CM) {
    resetMotors();
    if (cm > 0) setSpeed(speed, speed);
    else setSpeed(-speed, -speed);
    double enc = abs((cm / (WR * PI)) * 360);
    do {
      RightMotor.updateCurPos();
      LeftMotor.updateCurPos();
    } while (abs(RightMotor.getCurPos()) < (enc) && abs(LeftMotor.getCurPos()) < enc);
    resetMotors();
  } else {
    resetMotors();
    if (cm > 0) setSpeed(speed, -speed);
    else setSpeed(-speed, speed);
    double enc = abs((WB * (cm / WR)));
    do {
      Serial.println(LeftMotor.getCurPos());
      RightMotor.updateCurPos();
      LeftMotor.updateCurPos();
    } while ((abs(RightMotor.getCurPos()) < (enc) && abs(LeftMotor.getCurPos()) < enc));
    resetMotors();
  }
}
void resetMotors() {
  setSpeed(0.0, 0.0);
  RightMotor.setPulsePos(0);
  LeftMotor.setPulsePos(0);
  RightMotor.updateCurPos();
  LeftMotor.updateCurPos();
}
void followLine() {
  uint8_t line_value = lineFollower.readSensors();
  int speed = 0.4;
  int ang = 2;
  int cm = 1;
  switch (line_value) {
    case LB_RB: move(0.5, 0.3, MOVEMENT::CM); break;
    case LW_RB: move(3, 0.3, MOVEMENT::ANG); break;
    case LB_RW: move(-3, 0.3, MOVEMENT::ANG); break;
    case LW_RW: move(0.5, 0.3, MOVEMENT::CM); break;
  }
}
void gyroTurn(double ang, double speed) {
  gyro.begin();
  gyro.update();
  if (ang < 0) {
    setSpeed(-speed, speed);
    while (gyro.getAngleZ() > ang && gyro.getAngleZ() < 0 || gyro.getAngleZ() > -5 && gyro.getAngleZ() < 5) gyro.update();
  } else {
    setSpeed(speed, -speed);
    while (gyro.getAngleZ() < ang && gyro.getAngleZ() > 0 || gyro.getAngleZ() < -5 && gyro.getAngleZ() > 5) gyro.update();
  }
  setSpeed(0.0, 0.0);
}
void irAvoid(double r1, double speed) {
  double q, r2 = 7.3;
  bool left;
  q = r2 - (WBL / 2) + (WBW / 2);
  setSpeed(0.2, 0.2);
  while (usSensor.distanceCm() > q);
  setSpeed(0.0, 0.0);
  delay(100);
  gyroTurn(-90, 0.5);
  if (usSensor.distanceCm() > 20) left = true;
  else {
    left = false;
    gyroTurn(180, 0.5);
  }
  double lspeed, rspeed;
  lspeed = speed;
  rspeed = ((r1 + r2) / (r1 + r2 + WBL) * speed);
  if (left == false) setSpeed(rspeed, lspeed);
  else setSpeed(lspeed, rspeed);
  while (lineFollower.readSensors() == LW_RW);
  setSpeed(0.0, 0.0);
  delay(1000);
  gyroTurn(90, 0.5);
  delay(1000);
}
#endif