#include <Arduino.h>
#include <MeAuriga.h>
MePort_Sig mePort[17] = { {NC, NC}, {5, 4}, {3, 2}, {7, 6}, {9, 8}, {16, 17}, {A10, A15}, {A9, A14}, {A8, A13}, {A7, A12}, {A6, A11}, {NC, A2}, {NC, A3}, {NC, A0}, {NC, A1}, {NC, NC}, {NC, NC} };
Encoder_port_type encoder_Port[6] = { {NC, NC, NC, NC, NC}, {19, 42, 11, 49, 48}, {18, 43, 10, 47, 46}, {NC, NC, NC, NC, NC}, {NC, NC, NC, NC, NC} };
MeEncoderOnBoard RightMotor(SLOT1);
MeEncoderOnBoard LeftMotor(SLOT2);
void setSpeed(double left, double right)
{
  LeftMotor.setMotorPwm(left * 255);
  RightMotor.setMotorPwm(-right * 255);
}
void setSpeed(double val, bool left)
{
  if (left) LeftMotor.setMotorPwm(val * 255);
  if (left) RightMotor.setMotorPwm(-val * 255);
}
void isr_LeftMotorEncoder(void)
{
  if (digitalRead(LeftMotor.getPortB()) == 0) LeftMotor.pulsePosMinus();
  else LeftMotor.pulsePosPlus();
}
void isr_RightMotorEncoder(void)
{
  if (digitalRead(RightMotor.getPortB()) == 0) RightMotor.pulsePosMinus();
  else RightMotor.pulsePosPlus();
}
void setup()
{
  Serial.begin(9600);
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(LeftMotor.getIntNum(), isr_LeftMotorEncoder, RISING);
  attachInterrupt(RightMotor.getIntNum(), isr_RightMotorEncoder, RISING);
}
void loop()
{
  setSpeed(0.5, 0.5);
  while (RightMotor.getCurPos() < 3600 && LeftMotor.getCurPos() < 3600);
  {
    RightMotor.updateCurPos();
    LeftMotor.updateCurPos();
  }
  setSpeed(0.0, 0.0);
  delay(1000);
}