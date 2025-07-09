#include "DCMotor.h"
#include <Arduino.h>
#define LOW 0
#define HIGH 1
DCMotor::DCMotor(){};
DCMotor::~DCMotor(){};
void DCMotor::attach(unsigned int IN1, unsigned int IN2, unsigned int PWM) {
  in1 = IN1;
  in2 = IN2;
  pwm = PWM;
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(pwm, OUTPUT);
  isAttached = 1;
}
void DCMotor::invertDirection() {
  if (!isAttached) return;
  isInvertdirection = !isInvertdirection;
}
void DCMotor::setSpeed(bool direction, unsigned int value) {
  if (!isAttached) return;
  digitalWrite(in1, direction ^ isInvertdirection);
  digitalWrite(in2, !direction ^ isInvertdirection);
  if (value > 255) value = 255;
  analogWrite(pwm, value);
  isSetdirection = 1;
}
void DCMotor::setSpeed(unsigned int value) {
  if (!isAttached) return;
  if (isSetdirection)
    analogWrite(pwm, value);
}
void DCMotor::brake() {
  if (!isAttached) return;
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  isSetdirection = 0;
}
void DCMotor::stop() {
  if (!isAttached) return;
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  isSetdirection = 0;
}
