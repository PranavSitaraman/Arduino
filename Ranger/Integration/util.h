#ifndef UTIL_H
#define UTIL_H
void setSpeed(double, double);
void setSpeed(double, bool);
void isr_LeftMotorEncoder(void);
void isr_RightMotorEncoder(void);
void initPWM8kHz(void);
void move(double, double, uint8_t);
void resetMotors(void);
void gyroTurn(double, double);
void followLine(void);
void irAvoid(double r1, double speed);
#endif