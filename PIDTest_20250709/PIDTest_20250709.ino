#include "CLMotor.h"
#include "WemosD1R32_pins.h"
hw_timer_t* timer0;
CLMotor L_Motor;
CLMotor R_Motor;
//定时器中断
void IRAM_ATTR timer0_IRQ() {
  L_Motor.update();
  R_Motor.update();
}
//初始化
void setup() {
  //打开串口
  Serial.begin(115200);
  //左闭环电机初始化
  L_Motor.attach(LDIR1, LDIR2, LPWM, L_ENA, L_ENB, 0.05);
  L_Motor.setPID(2, 5, 0);
  //右闭环电机初始化
  R_Motor.attach(RDIR1, RDIR2, RPWM, R_ENA, R_ENB, 0.05);
  R_Motor.setPID(2, 0, 0);
  //打开定时器，定时器开始之后会自动开始电机测速
  timer0 = timerBegin(10000);
  if ( timer0 == NULL) {
    Serial.println("定时器配置失败");
  } else {
    Serial.println("定时器配置成功");
    timerAttachInterrupt(timer0, timer0_IRQ);
    timerAlarm(timer0, 500, true, 0);
    timerStart(timer0);
  }
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
  Serial.print("Target Speed:");
  Serial.print(L_Motor.getTargetSpeed());
  Serial.print(" rpm Actual Speed:");
  Serial.print(L_Motor.getSpeed());
  Serial.print(" rpm PWM Value:");
  Serial.println(L_Motor.getPWM());
  delay(100);
}
