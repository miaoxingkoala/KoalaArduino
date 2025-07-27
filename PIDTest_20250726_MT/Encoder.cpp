#include "Encoder.h"
#include <Arduino.h>
Encoder::Encoder() {
}
Encoder::~Encoder() {
}
void Encoder::attach(unsigned int apin, unsigned int bpin) {
  A_pin = apin;
  B_pin = bpin;
  pinMode(A_pin, INPUT);
  pinMode(B_pin, INPUT);
  attachInterruptArg(A_pin, ENA_IRQ, this, CHANGE);
  attachInterruptArg(B_pin, ENB_IRQ, this, CHANGE);
}
void Encoder::updateA() {
  if (digitalRead(A_pin) ^ digitalRead(B_pin)) {
    encoderCount--;
  } else {
    encoderCount++;
  }
  //MT测速新添加
    //如果sampleSpeed为1，则采样当前计数器值，并和上次计数器值做差得到计数器差值;
    //采样当前编码器值，和上次编码器值做差得到编码器差值
    if (sampleSpeed) {
      timerLastCount = timerThisCount;
      timerThisCount = timerRead(timer_H);
      timerRelativeCount = (timerThisCount - timerLastCount);
      encoderLastCount = encoderThisCount;
      encoderThisCount = getCount();
      encoderRelativeCount = encoderThisCount - encoderLastCount;
      sampleSpeed = 0;
    }
    //
}
void Encoder::updateB() {
  if (digitalRead(A_pin) ^ digitalRead(B_pin)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
  //MT测速新添加
    //如果sampleSpeed为1，则采样当前计数器值，并和上次计数器值做差得到计数器差值;采样当前编码器值，和上次编码器值做差得到编码器差值
    if (sampleSpeed) {
      timerLastCount = timerThisCount;
      timerThisCount = timerRead(timer_H);
      timerRelativeCount = (timerThisCount - timerLastCount);
      encoderLastCount = encoderThisCount;
      encoderThisCount = getCount();
      encoderRelativeCount = encoderThisCount - encoderLastCount;
      sampleSpeed = 0;
    }
    //
}
long long Encoder::getCount() {
  return encoderCount;
}
void Encoder::setCount(long long count) {
  encoderCount = count;
}
void Encoder::clear() {
  encoderCount = 0;
}