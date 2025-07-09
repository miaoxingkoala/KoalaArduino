#include "CLMotor.h"
#include <Arduino.h>
#define LOW 0
#define HIGH 1

CLMotor::CLMotor() {
}
CLMotor::~CLMotor() {
}
void CLMotor::attach(unsigned int in1, unsigned int in2, unsigned int pwm, unsigned int en1, unsigned int en2, double timerinterval){
  m_motor.attach(in1, in2, pwm);
  isAttachedMotor = 1;
  m_encoder.attach(en1, en2);
  isAttachedEncoder = 1;
  setTimerInterval(timerinterval);
}
void CLMotor::attachMotor(unsigned int in1, unsigned int in2, unsigned int pwm) {
  m_motor.attach(in1, in2, pwm);
  isAttachedMotor = 1;
}
void CLMotor::attachEncoder(unsigned int en1, unsigned int en2) {
  m_encoder.attach(en1, en2);
  isAttachedEncoder = 1;
}
void CLMotor::setPID(double _kp, double _ki, double _kd) {
  if (_kp > 10) _kp = 10;
  if (_ki > 10) _ki = 10;
  if (_kd > 5) _kd = 5;
  if (_kp < 0) _kp = 0;
  if (_ki < 0) _ki = 0;
  if (_kd < 0) _kd = 0;
  kp = _kp;
  ki = _ki;
  kd = _kd;
}
void CLMotor::setTargetSpeed(double targetspeed) {
  if (targetspeed > 300) targetspeed = 300;
  if (targetspeed < -300) targetspeed = 300;
  targetSpeed = targetspeed;
}
void CLMotor::setTimerInterval(double time) {
  timerInterval = time;
}
void CLMotor::invertDirection() {
  m_motor.invertDirection();
}
void CLMotor::updateSpeed() {
  encoderCount = m_encoder.getCount();
  angle = encoderCount * 360.0 / 1320;
  speed = (encoderCount - encoderLastCount) * 60.0 / 1320 / timerInterval;
  encoderLastCount = encoderCount;
}
void CLMotor::updateError() {
  speedError = targetSpeed - speed;
  speedErrorDerivative = (speedError - lastSpeedError) / timerInterval;
  speedErrorIntegral += speedError * timerInterval;
  if (speedErrorIntegral > 200) speedErrorIntegral = 200;
  if (speedErrorIntegral < -200) speedErrorIntegral = -200;
  lastSpeedError = speedError;
}
void CLMotor::calculatePID() {
  PIDOutput = kp * speedError + ki * speedErrorIntegral + kd * speedErrorDerivative;
  if (PIDOutput > 255) PIDOutput = 255;
  if (PIDOutput < -255) PIDOutput = -255;
}
void CLMotor::outputPWM() {
  if (PIDOutput < 1 && PIDOutput > -1) {
    m_motor.brake();
    return;
  }
  if (PIDOutput > 0) {
    PWMValue = PIDOutput;
    m_motor.setSpeed(1, PWMValue);
  } else if (PIDOutput < 0) {
    PWMValue = -PIDOutput;
    m_motor.setSpeed(0, PWMValue);
  } else {
    m_motor.brake();
    PWMValue = 0;
  }
}
double CLMotor::getTargetSpeed() {
  return targetSpeed;
}
double CLMotor::getSpeed() {
  return speed;
}
int CLMotor::getPWM() {
  return PWMValue;
}
void CLMotor::update() {
  updateSpeed();
  updateError();
  calculatePID();
  outputPWM();
}