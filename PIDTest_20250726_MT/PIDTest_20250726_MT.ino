#include "CLMotor.h"
#include "WemosD1R32_pins.h"
hw_timer_t* timer_L;
hw_timer_t* timer_H;
CLMotor L_Motor;
CLMotor R_Motor;
//定时器中断
void IRAM_ATTR timer_L_IRQ() {
  //
  L_Motor.update();
  R_Motor.update();
  //MT测速：在低频中断设置采样信号
  L_Motor.sampleSpeed();
  R_Motor.sampleSpeed();
}
//初始化
void setup() {
  //打开串口
  Serial.begin(115200);
  //左闭环电机初始化
  L_Motor.attach(LDIR1, LDIR2, LPWM, L_ENA, L_ENB);
  L_Motor.setPID(2, 5, 0);
  //右闭环电机初始化
  R_Motor.attach(RDIR1, RDIR2, RPWM, R_ENA, R_ENB);
  R_Motor.setPID(2, 0, 0);
  //打开定时器，定时器开始之后会自动开始电机测速
  //低频定时器，定时频率为10KHz
  timer_L = timerBegin(10000);
  if ( timer_L == NULL) {
    Serial.println("低速定时器配置失败");
  } else {
    Serial.println("低速定时器配置成功");
    timerAttachInterrupt(timer_L, timer_L_IRQ);
    //每隔20毫秒触发一次中断
    timerAlarm(timer_L, 200, true, 0);
    timerStart(timer_L);
  }
  //高频定时器，定时频率为10MHz
  timer_H = timerBegin(10000000);//10MHz
  if ( timer_H == NULL) {
    Serial.println("高速定时器配置失败");
  } else {
    Serial.println("高速定时器配置成功");
    //高速定时器只要一直递增就可以了
  }
  //将高频定时器指针传给电机
  L_Motor.setTimerHandle(timer_H);
  R_Motor.setTimerHandle(timer_H);
  //接下来只要给电机写速度就可以了，是闭环的
  L_Motor.setTargetSpeed(60);
  R_Motor.setTargetSpeed(-60);
}

void loop() {
  if (Serial.available()) {
    delay(5);
    double speedIn = Serial.parseFloat();
    L_Motor.setTargetSpeed(speedIn);
    R_Motor.setTargetSpeed(speedIn);
  }
  //优化了输出函数，每0.1秒输出一次，五个数据平均实现平滑
  double L_speed =0;
  int L_PWM = 0;
  for(int i=1;i<=5;i++){
    L_speed += L_Motor.getSpeed();
    L_PWM += L_Motor.getPWM();
    delay(20);
  }
  L_speed/=5;
  L_PWM/=5;
  Serial.print("Target Speed:");
  Serial.print(L_Motor.getTargetSpeed());
  Serial.print(" rpm Actual Speed:");
  Serial.print(L_speed);
  Serial.print(" rpm PWMValue:");
  Serial.println(L_PWM);
  //Serial.println(L_Motor.timerInterval);
  
}

