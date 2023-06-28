#include<Wire.h>
#include<Arduino.h>
#include <Adafruit_MotorShield.h>
volatile int encoder[] = {0, 0, 0, 0};
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
void setup()
{
  Serial.begin(9600);
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  if (!AFMS.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  attachInterrupt(digitalPinToInterrupt(2), update1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), update2, RISING);
  attachInterrupt(digitalPinToInterrupt(18), update3, RISING);
  attachInterrupt(digitalPinToInterrupt(19), update4, RISING);
  motor1->setSpeed(100);
  motor1->run(FORWARD);
  motor2->setSpeed(100);
  motor2->run(FORWARD);
  motor3->setSpeed(100);
  motor3->run(FORWARD);
  motor4->setSpeed(100);
  motor4->run(FORWARD);
}
void loop()
{
  Serial.print(encoder[0]);
  Serial.print("\t");
  Serial.print(encoder[1]);
  Serial.print("\t");
  Serial.print(encoder[2]);
  Serial.print("\t");
  Serial.println(encoder[3]);
}
void update1()
{
  encoder[0] += 1;
}
void update2()
{
  encoder[1] += 1;
}
void update3()
{
  encoder[2] += 1;
}
void update4()
{
  encoder[3] += 1;
}
