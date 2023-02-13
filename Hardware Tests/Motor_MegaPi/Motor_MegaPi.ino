#include "MeMegaPi.h"

MeMegaPiDCMotor motor1(PORT2B);

MeMegaPiDCMotor motor2(PORT1B);

uint8_t motorSpeed = 100;

void setup()
{
}

void loop()
{
  motor1.run(motorSpeed); /* value: between -255 and 255. */
  motor2.run(motorSpeed); /* value: between -255 and 255. */
  delay(2000);
  motor1.stop();
  motor2.stop();
  delay(2000);
  motor1.run(-motorSpeed);
  motor2.run(-motorSpeed);
  delay(2000);
  motor1.stop();
  motor2.stop();
  delay(2000);
}