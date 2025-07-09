#ifndef CLMotor_H
#define CLMotor_H
#include "Encoder.h"
#include "DCMotor.h"

class CLMotor{
  public:
  CLMotor();
  ~CLMotor();
  void attach(unsigned int in1,unsigned int in2,unsigned int pwm,unsigned int en1,unsigned int en2,double timerinterval);
  void attachMotor(unsigned int in1,unsigned int in2,unsigned int pwm);
  void attachEncoder(unsigned int en1,unsigned int en2);
  void setPID(double _kp,double _ki,double _kd);
  void setTargetSpeed(double targetspeed);
  void setTimerInterval(double time);
  void invertDirection();
  void updateSpeed();
  void updateError();
  void calculatePID();
  void outputPWM();
  double getTargetSpeed();
  double getSpeed();
  void update();
  int getPWM();
  Encoder m_encoder;
  DCMotor m_motor;
  double kp = 2;
  double ki = 0;
  double kd = 0;
  private:
  long long encoderCount;
  long long encoderLastCount;
  bool isAttachedMotor = 0;
  bool isAttachedEncoder = 0;
  double angle;
  double speed;
  double targetSpeed;
  double lastSpeed;
  double lastSpeedError;
  double speedError;
  double speedErrorDerivative;
  double speedErrorIntegral;
  double timerInterval = 0.05;
  double PIDOutput;
  int PWMValue;
};

#endif
