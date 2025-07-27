#ifndef CLMotor_H
#define CLMotor_H
#include "Encoder.h"
#include "DCMotor.h"

class CLMotor{
  public:
  CLMotor();
  ~CLMotor();
  void attach(unsigned int in1,unsigned int in2,unsigned int pwm,
  unsigned int en1,unsigned int en2);
  void attachMotor(unsigned int in1,unsigned int in2,unsigned int pwm);
  void attachEncoder(unsigned int en1,unsigned int en2);
  void setPID(double _kp,double _ki,double _kd);
  void setTargetSpeed(double targetspeed);
  void invertDirection();
  void updateSpeed();
  void updateError();
  void calculatePID();
  void outputPWM();
  double getTargetSpeed();
  double getSpeed();
  void update();
  int getPWM();
  //MT测速添加
  //设置采样标记
  void sampleSpeed();
  //设置编码器的高频定时器指针
  void setTimerHandle(hw_timer_t* m_timer);
  Encoder m_encoder;//编码器类
  DCMotor m_motor;//电机类
  double kp = 2;//比例参数
  double ki = 0;//积分参数
  double kd = 0;//微分参数
  private:
  long long encoderCount;//编码器计数
  long long encoderLastCount;//编码器上次计数
  bool isAttachedMotor = 0;//是否连接电机
  bool isAttachedEncoder = 0;//是否连接编码器
  double angle;//电机转角
  double speed;//速度
  double targetSpeed;//目标速度
  double lastSpeed;//上次速度
  double lastSpeedError;//上次速度误差
  double speedError;//速度误差
  double speedErrorDerivative;//速度误差微分
  double speedErrorIntegral;//速度误差积分
  double timerInterval = 0.05;//定时器时间间距
  double PIDOutput;//PID输出值
  int PWMValue;//PWM值
  
};

#endif
