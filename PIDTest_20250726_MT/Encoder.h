#ifndef ENCODER_H
#define ENCODER_H
#include <esp_attr.h>
#include <esp32-hal-timer.h>
class Encoder {
public:
  Encoder();
  ~Encoder();
  static void IRAM_ATTR ENA_IRQ(void* encoder) {
    Encoder* m_encoder = (Encoder*)encoder;
    m_encoder->updateA();
  }
  static void IRAM_ATTR ENB_IRQ(void* encoder) {
    Encoder* m_encoder = (Encoder*)encoder;
    m_encoder->updateB();
  }
  void attach(unsigned int apin, unsigned int bpin);
  void updateA();
  void updateB();
  long long getCount();
  void setCount(long long count);
  void clear();
private:
  long long encoderCount = 0;

  unsigned int A_pin;
  unsigned int B_pin;
public:
  //MT测速添加
  hw_timer_t* timer_H = NULL; //高频定时器指针
  bool sampleSpeed = 0; //是否进行速度采样 标记位
  unsigned long long timerThisCount;  //记录高频定时器当前值
  unsigned long long timerLastCount;  //记录高频定时器上次值
  unsigned long long timerRelativeCount; //记录高频定时器相对值
  long long encoderLastCount = 0; //记录编码器上次值
  long long encoderThisCount = 0; //记录编码器当前值
  long long encoderRelativeCount = 0; //记录编码器相对值
};
#endif
