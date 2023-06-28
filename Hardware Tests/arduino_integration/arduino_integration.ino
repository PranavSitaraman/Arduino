#include <Adafruit_BNO055.h>
#include <Adafruit_MotorShield.h>
#include <Arduino.h>
#include <VL53L0X.h>
#include <Wire.h>
#include <Servo.h>
namespace Dir
{
  enum : uint8_t { N, E, S, W };
}
namespace Coord
{
  enum : uint8_t { X, Y, Z };
}
namespace Move
{
  enum Move : uint8_t { FAILURE, SUCCESS, WALL };
}
constexpr auto RESET = Dir::W + 1;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motors[4] = {AFMS.getMotor(1), AFMS.getMotor(2), AFMS.getMotor(3), AFMS.getMotor(4)};
VL53L0X tof;
Adafruit_BNO055 bno(55);
Servo servo;
constexpr uint8_t VLX[] {3, 1, 2, 4};
constexpr uint8_t BOS[] {5, 6};
constexpr uint8_t ENC = 2;
constexpr uint8_t DIST_THRESH = 10;
constexpr uint8_t DIST_THRESH2 = 5;
constexpr uint8_t LED = 12;
constexpr double WHEEL_RAD = 3.25;
volatile uint16_t encoder[4] {};
void encoderISR() {
  encoder[0]++;
}
void tcaselect(uint16_t i)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
}
void motorReset()
{
  for (const auto &motor : motors)
  {
    motor->run(RELEASE);
  }
  for (uint8_t i = 0; i < sizeof(encoder) / sizeof(encoder[0]); i++)
  {
    encoder[i] = 0;
  }
}
void (*resetFunc)(void) = 0;
void drop(int side)
{
  if (side == 0)
  {
    servo.write(120);
    delay(1000);
    servo.write(45);
    delay(1000);
    servo.write(60);
  }
  else
  {
    servo.write(0);
    delay(1000);
    servo.write(75);
    delay(1000);
    servo.write(60);
  }
}
void handleVictim()
{
  uint8_t val = Serial1.read();
  uint8_t side = Serial1.read();
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED, LOW);
  for (uint8_t i = 0; i < val; i++)
  {
    drop(side);
  }
}
uint16_t distance(uint16_t port = VLX[0])
{
  tcaselect(port);
  return tof.readRangeContinuousMillimeters();
}
int16_t orientation(uint8_t coord, uint16_t port = BOS[0])
{
  tcaselect(port);
  sensors_event_t event;
  bno.getEvent(&event);
  if (coord == Coord::X)
  {
    return event.orientation.x;
  }
  if (coord == Coord::Y)
  {
    return event.orientation.y;
  }
  return event.orientation.z;
}
Move::Move move(const bool dir[4], double a, double motorSpeed)
{
  static constexpr uint16_t kp = 2;
  double b = motorSpeed;
  motorSpeed *= 255;
  for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++)
  {
    motors[i]->setSpeed(motorSpeed);
    motors[i]->run(dir[i] ? FORWARD : BACKWARD);
  }
  while (encoder[0] < ((75 * 48 * a) / (2 * PI * WHEEL_RAD)) * (0.25) / (b))
  {
    if (abs(orientation(Coord::Y, BOS[1])) > 15)
    {
      while (abs(orientation(Coord::Y, BOS[1])) > 15)
      {
        if (orientation(Coord::Y, BOS[1]) < -15)
        {
          for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++)
          {
            motors[i]->setSpeed(motorSpeed * 0.5);
          }
        }
        else
        {
          for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++)
          {
            motors[i]->setSpeed(motorSpeed * 3.0 / 2.0);
          }
        }
      }
      motorReset();
      return Move::WALL;
    }
    uint16_t left = distance(VLX[Dir::W]) / 10;
    uint16_t right = distance(VLX[Dir::E]) / 10;
    uint16_t up = distance(VLX[Dir::N]) / 10;
    uint16_t down = distance(VLX[Dir::S]) / 10;
    if (dir[0] && up < DIST_THRESH2) break;
    if (!dir[0] && down < DIST_THRESH2) break;
    if (left <= 2 * DIST_THRESH && right <= 2 * DIST_THRESH)
    {
      motors[0]->setSpeed(motorSpeed + kp * (left - right));
      motors[3]->setSpeed(motorSpeed + kp * (left - right));
      motors[1]->setSpeed(motorSpeed + kp * (right - left));
      motors[2]->setSpeed(motorSpeed + kp * (right - left));
    }
    else if (left <= 2 * DIST_THRESH && right > 2 * DIST_THRESH)
    {
      motors[0]->setSpeed(motorSpeed + kp * (left - DIST_THRESH));
      motors[3]->setSpeed(motorSpeed + kp * (left - DIST_THRESH));
      motors[1]->setSpeed(motorSpeed + kp * (DIST_THRESH - left));
      motors[2]->setSpeed(motorSpeed + kp * (DIST_THRESH - left));
    }
    else if (left > 2 * DIST_THRESH && right <= 2 * DIST_THRESH)
    {
      motors[0]->setSpeed(motorSpeed + kp * (DIST_THRESH - right));
      motors[3]->setSpeed(motorSpeed + kp * (DIST_THRESH - right));
      motors[1]->setSpeed(motorSpeed + kp * (right - DIST_THRESH));
      motors[2]->setSpeed(motorSpeed + kp * (right - DIST_THRESH));
    }
    else
    {
      for (const auto &motor : motors)
      {
        motor->setSpeed(motorSpeed);
      }
    }
    switch (Serial1.read())
    {
      case 0:
        {
          for (const auto &motor : motors)
          {
            motor->run(RELEASE);
          }
          handleVictim();
          for (uint8_t i = 0; i < sizeof(motors) / sizeof(*motors); i++)
          {
            motors[i]->setSpeed(motorSpeed);
            motors[i]->run(dir[i] ? FORWARD : BACKWARD);
          }
          break;
        }
      case 1:
        {
          uint16_t reverse = encoder[0];
          motorReset();
          for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++)
          {
            motors[i]->setSpeed(motorSpeed);
            motors[i]->run(dir[i] ? BACKWARD : FORWARD);
          }
          while (encoder[0] < reverse);
          motorReset();
          Serial1.write((uint8_t)1);
          return Move::FAILURE;
        }
    }
  }
  uint16_t up = distance(VLX[Dir::N]) / 10;
  uint16_t down = distance(VLX[Dir::S]) / 10;
  if (dir[0] && up < 2 * DIST_THRESH2) while (up > DIST_THRESH2) up = distance(VLX[Dir::N]) / 10;
  else if (!dir[0] && down < 2 * DIST_THRESH2) while (down > DIST_THRESH2) down = distance(VLX[Dir::S]) / 10;
  motorReset();
  return Move::SUCCESS;
}
bool forward(double a = 35, double motorSpeed = 0.5)
{
  static constexpr bool dir[] {true, false, false, true};
  return move(dir, a, motorSpeed);
}
bool backward(double a = 35, double motorSpeed = 0.5)
{
  static constexpr bool dir[] {false, true, true, false};
  return move(dir, a, motorSpeed);
}
void turn(const bool dir[4], double a, double motorSpeed, uint16_t port)
{
  for (uint16_t i = 0; i < sizeof(motors) / sizeof(*motors); i++)
  {
    motors[i]->setSpeed(motorSpeed);
    motors[i]->run(dir[i] ? FORWARD : BACKWARD);
  }
  int16_t start = orientation(Coord::X, port);
  while (abs((orientation(Coord::X, port) - start + 540) % 360 - 180) < a);
  motorReset();
}
void left(double a = 90, double motorSpeed = 0.5, uint16_t port = BOS[0])
{
  static constexpr bool dir[4] {true, true, true, true};
  turn(dir, a, motorSpeed * 255, port);
}
void right(double a = 90, double motorSpeed = 0.5, uint16_t port = BOS[0])
{
  static constexpr bool dir[4] {};
  turn(dir, a, motorSpeed * 255, port);
}
void setup()
{
  pinMode(6, OUTPUT);
  analogWrite(6, 168);
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  AFMS.begin();
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
  attachInterrupt(digitalPinToInterrupt(ENC), &encoderISR, RISING);
  servo.attach(9);
  servo.write(60);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED, LOW);
  motorReset();
  Serial.write((uint8_t) 1);
}
void loop()
{
  if (Serial.available())
  {
    uint8_t command = Serial.read();
    if (command >> 7)
    {
      command &= ~(1 << 7);
      uint8_t state;
      switch (command)
      {
        case Dir::N:
          {
            state = forward();
            break;
          }
        case Dir::E:
          {
            right();
            state = forward();
            break;
          }
        case Dir::S:
          {
            state = backward();
            break;
          }
        case Dir::W:
          {
            left();
            state = forward();
            break;
          }
        case RESET:
          {
            resetFunc();
            break;
          }
      }
      Serial.write(state);
    }
    else
    {
      uint16_t val = distance(VLX[command]);
      Serial.write((uint8_t *)&val, sizeof(val));
    }
  }
}
