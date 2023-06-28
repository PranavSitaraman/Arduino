#include <Adafruit_BNO055.h>
#include <Adafruit_MotorShield.h>
#include <Arduino.h>
#include <VL53L0X.h>
#include <Wire.h>
namespace Dir { enum : uint8_t {N, E, S, W}; }
namespace Col { enum : uint8_t {R, G, B}; }
namespace Coord { enum : uint8_t {X, Y, Z}; }
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motors[4] = {AFMS.getMotor(1), AFMS.getMotor(2), AFMS.getMotor(3), AFMS.getMotor(4)};
VL53L0X tof; Adafruit_BNO055 bno(55);
constexpr uint16_t VLX[] {3, 1, 2, 4}, BOS[] {5, 6}, DROP[] = {11, 12, 10};
constexpr uint16_t ENC = 2, distThresh = 10, kp = 2, piInterrupt = 18, colorInterrupt = 19, LED = 13;
constexpr double wheelRad = 3.25;
volatile uint16_t encoder[4] {};
volatile bool dir[4] {};
volatile uint8_t state = LOW, colorState = LOW;
void update(uint16_t n) { encoder[n]++; }
void tcaselect(uint16_t i)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
}
void motorReset()
{
  for (const auto& motor : motors) motor->run(RELEASE);
  for (int i = 0; i < sizeof(encoder)/sizeof(encoder[0]); i++) encoder[i] = 0;
}
void drop(int dir)
{
  
}
void pi() { state = HIGH; }
void color() { colorState = HIGH; }
uint16_t distance(uint16_t port = VLX[0])
{
  tcaselect(port);
  return tof.readRangeContinuousMillimeters();
}
int16_t orientation(uint16_t coord, uint16_t port = BOS[0])
{
  tcaselect(port);
  sensors_event_t event;
  bno.getEvent(&event);
  if (coord == Coord::X) return event.orientation.x;
  if (coord == Coord::Y) return event.orientation.y;
  return event.orientation.z;
}
void move(double a, double motorSpeed)
{
  double b = motorSpeed; motorSpeed *= 255;
  for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++) { motors[i]->setSpeed(motorSpeed); motors[i]->run(dir[i] ? FORWARD : BACKWARD); }
  while (encoder[0] < ((75 * 48 * a) / (2 * PI * wheelRad)) * (0.25) / (b))
  {
    uint16_t left = distance(VLX[Dir::W])/10, right = distance(VLX[Dir::E])/10;
    if (left <= 2 * distThresh && right <= 2 * distThresh)
    {
      motors[0]->setSpeed(motorSpeed + kp * (left - right));
      motors[3]->setSpeed(motorSpeed + kp * (left - right));
      motors[1]->setSpeed(motorSpeed + kp * (right - left));
      motors[2]->setSpeed(motorSpeed + kp * (right - left));
    }
    else if (left <= 2 * distThresh && right > 2 * distThresh)
    {
      motors[0]->setSpeed(motorSpeed + kp * (left - distThresh));
      motors[3]->setSpeed(motorSpeed + kp * (left - distThresh));
      motors[1]->setSpeed(motorSpeed + kp * (distThresh - left));
      motors[2]->setSpeed(motorSpeed + kp * (distThresh - left));
    }
    else if (left > 2 * distThresh && right <= 2 * distThresh)
    {
      motors[0]->setSpeed(motorSpeed + kp * (distThresh - right));
      motors[3]->setSpeed(motorSpeed + kp * (distThresh - right));
      motors[1]->setSpeed(motorSpeed + kp * (right - distThresh));
      motors[2]->setSpeed(motorSpeed + kp * (right - distThresh));
    }
    else
    {
      motors[0]->setSpeed(motorSpeed);
      motors[1]->setSpeed(motorSpeed);
      motors[2]->setSpeed(motorSpeed);
      motors[3]->setSpeed(motorSpeed);
    }
    if (state == HIGH)
    {
      for (const auto& motor : motors) motor->run(RELEASE);
      int val = digitalRead(DROP[0]) * 2 + digitalRead(DROP[1]);
      int dir1 = digitalRead(DROP[2]);
      digitalWrite(LED, HIGH);
      delay(5000);
      digitalWrite(LED, LOW);
      for (int i = 0; i < val; i++) drop(dir1);
      for (int i = 0; i < sizeof(motors) / sizeof(*motors); i++) { motors[i]->setSpeed(motorSpeed); motors[i]->run(dir[i] ? FORWARD : BACKWARD); }
      state = LOW;
    }
    if (colorState == HIGH)
    {
      uint16_t reverse = encoder[0];
      motorReset();
      for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++) { motors[i]->setSpeed(motorSpeed); motors[i]->run(dir[i] ? BACKWARD : FORWARD); }
      while (encoder[0] < reverse);
      colorState = LOW;
      break;
    }
  }
  motorReset();
}
void forward(double a = 30, double motorSpeed = 0.25)
{
  static constexpr bool fwd_dir[] {true, false, false, true};
  for (int i = 0; i < sizeof(dir)/sizeof(dir[0]); i++) dir[i] = fwd_dir[i];
  move(a, motorSpeed);
}
void backward(double a = 30, double motorSpeed = 0.25)
{
  static constexpr bool back_dir[] {false, true, true, false};
  for (int i = 0; i < sizeof(dir)/sizeof(dir[0]); i++) dir[i] = back_dir[i];
  move(a, motorSpeed);
}
void turn(double a, double motorSpeed, uint16_t port)
{
  for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++) { motors[i]->setSpeed(motorSpeed); motors[i]->run(dir[i] ? FORWARD : BACKWARD); }
  int16_t start = orientation(Coord::X, port);
  while (true)
  {
    if (abs((orientation(Coord::X, port) - start + 540) % 360 - 180) >= a) break;
    if (state == HIGH)
    {
      for (const auto& motor : motors) motor->run(RELEASE);
      int val = digitalRead(DROP[0]) * 2 + digitalRead(DROP[1]);
      int dir1 = digitalRead(DROP[2]);
      digitalWrite(LED, HIGH);
      delay(5000);
      digitalWrite(LED, LOW);
      for (int i = 0; i < val; i++) drop(dir1);
      for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++) { motors[i]->setSpeed(motorSpeed); motors[i]->run(dir[i] ? FORWARD : BACKWARD); }
      state = LOW;
    }
  }
  motorReset();
}
void left(double a = 90, double motorSpeed = 0.25, uint16_t port = BOS[0])
{
  for (int i = 0; i < sizeof(dir)/sizeof(dir[0]); i++) dir[i] = 1;
  turn(a, motorSpeed * 255, port);
}
void right(double a = 90, double motorSpeed = 0.25, uint16_t port = BOS[0])
{
  for (int i = 0; i < sizeof(dir)/sizeof(dir[0]); i++) dir[i] = 0;
  turn(a, motorSpeed * 255, port);
}
void setup()
{
  Serial.begin(9600); Wire.begin(); AFMS.begin();
  for (auto port : BOS)
  {
    tcaselect(port);
    bno.begin();
  }
  for (auto port : VLX)
  {
    tcaselect(port);
    tof.init();
    tof.setTimeout(500);
    tof.startContinuous();
  }
  attachInterrupt(digitalPinToInterrupt(ENC), [] { update(0); }, RISING);
  attachInterrupt(digitalPinToInterrupt(piInterrupt), pi, RISING);
  attachInterrupt(digitalPinToInterrupt(colorInterrupt), color, RISING);
  pinMode(LED, OUTPUT);
  pinMode(DROP[0], INPUT);
  pinMode(DROP[1], INPUT);
  pinMode(DROP[2], INPUT);
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED, LOW);
  motorReset();
}
void loop()
{
  if (Serial.available())
  {
    uint8_t command = Serial.read();
    if (command >> 7)
    {
      command &= ~(1 << 7);
      switch (command)
      {
        case Dir::N: forward(); break;
        case Dir::E: right(); forward(); break;
        case Dir::S: backward(); break;
        case Dir::W: left(); forward(); break;
      }
      Serial.write((uint8_t)1);
    }
    else
    {
      uint16_t val = distance(VLX[command]);
      Serial.write((uint8_t*)&val, sizeof(val));
    }
  }
}
