#include <Arduino.h>
#include <Wire.h>
#include <MeAuriga.h>
#define lMotorPwm 11
#define lMotor1 48
#define lMotor2 49
#define lInterrupt 19
#define lChannelB 43
#define rMotorPwm 10
#define rMotor1 47
#define rMotor2 46
#define rInterrupt 18
#define rChannelB 42
#define MAX_ACC 20.0
double lTarPwm = 0;
double lCurPwm = 0;
double rTarPwm = 0;
double rCurPwm = 0;
int lEnc = 0;
int rEnc = 0;
MePort_Sig mePort[17] = { {NC, NC}, {5, 4}, {3, 2}, {7, 6}, {9, 8}, {16, 17}, {A10, A15}, {A9, A14}, {A8, A13}, {A7, A12}, {A6, A11}, {NC, A2}, {NC, A3}, {NC, A0}, {NC, A1}, {NC, NC}, {NC, NC} };
Encoder_port_type encoder_Port[6] = { {NC, NC, NC, NC, NC}, {19, 42, 11, 49, 48}, {18, 43, 10, 47, 46}, {NC, NC, NC, NC, NC}, {NC, NC, NC, NC, NC} };
void setRawMotorSpeeds(double left, double right)
{
  lCurPwm = left;
  if (left < 0)
  {
    analogWrite(lMotorPwm, -left * 200);
    digitalWrite(lMotor1, LOW);
    digitalWrite(lMotor2, HIGH);
  }
  else
  {
    analogWrite(lMotorPwm, left * 200);
    digitalWrite(lMotor1, HIGH);
    digitalWrite(lMotor2, LOW);
  }
  rCurPwm = right;
  if (right < 0)
  {
    analogWrite(rMotorPwm, -right * 200);
    digitalWrite(rMotor1, LOW);
    digitalWrite(rMotor2, HIGH);
  }
  else
  {
    analogWrite(rMotorPwm, right * 200);
    digitalWrite(rMotor1, HIGH);
    digitalWrite(rMotor2, LOW);
  }
}
void lcount(void)
{
  lEnc += 1;
}
void rcount(void)
{
  rEnc += 1;
}
float a = 0;
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(lMotorPwm, OUTPUT);
  pinMode(lMotor1, OUTPUT);
  pinMode(lMotor2, OUTPUT);
  pinMode(lInterrupt, INPUT_PULLUP);
  pinMode(lChannelB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lInterrupt), lcount, FALLING);
  pinMode(rMotorPwm, OUTPUT);
  pinMode(rMotor1, OUTPUT);
  pinMode(rMotor2, OUTPUT);
  pinMode(rInterrupt, INPUT_PULLUP);
  pinMode(rChannelB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rInterrupt), rcount, FALLING);
}
void loop()
{
  a += 0.05;
  Serial.println(sin(a));
  setRawMotorSpeeds(0.25 * sin(a) + 0.5, 0.5 - 0.25 * sin(a));
}
