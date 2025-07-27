#include "CLMotor.h"
#include <Arduino.h>
#define LOW 0
#define HIGH 1

CLMotor::CLMotor() {
}
CLMotor::~CLMotor() {
}
void CLMotor::attach(unsigned int in1, unsigned int in2, 
  unsigned int pwm, unsigned int en1, unsigned int en2) {
  m_motor.attach(in1, in2, pwm);
  isAttachedMotor = 1;
  m_encoder.attach(en1, en2);
  isAttachedEncoder = 1;
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
  if (targetspeed > 400) targetspeed = 400;
  if (targetspeed < -400) targetspeed = -400;
  targetSpeed = targetspeed;
}

void CLMotor::invertDirection() {
  m_motor.invertDirection();
}
void CLMotor::updateSpeed() {
  // encoderCount = m_encoder.getCount();
  // angle = encoderCount * 360.0 / 1320;
  // speed = (encoderCount - encoderLastCount) * 60.0 / 1320 / timerInterval;
  // encoderLastCount = encoderCount;
  //MT测速法更新：上一次的编码器中断已经把数据采集了进来，现在需要计算速度
  if (m_encoder.sampleSpeed == 0) {
    angle = m_encoder.encoderThisCount * 360.0 / 1320;
    timerInterval =  (double)m_encoder.timerRelativeCount / 10000000.0;
    if(timerInterval<0.01)timerInterval = 0.02;//避免电机启动时异常值
    speed = m_encoder.encoderRelativeCount * 60.0 / 1320 / timerInterval;
  } else {//如果samplespeed不为0，说明上一个定时器周期没有任何脉冲进入
    speed = 0;
    m_encoder.sampleSpeed = 0;
  }
}
void CLMotor::updateError() {
  speedError = targetSpeed - speed;
  speedErrorDerivative = (speedError - lastSpeedError) / timerInterval;
  speedErrorIntegral += speedError * timerInterval;
  if (speedErrorIntegral > 900) speedErrorIntegral = 900;
  if (speedErrorIntegral < -900) speedErrorIntegral = -900;
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

//MT测速添加
void CLMotor::sampleSpeed() {
  m_encoder.sampleSpeed = 1;
}

void CLMotor::setTimerHandle(hw_timer_t* m_timer) {
  m_encoder.timer_H = m_timer;
}