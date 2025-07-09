#ifndef ENCODER_H
#define ENCODER_H
#include <esp_attr.h>

// //中断服务函数,只能写在全局变量，不能放在类里
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
};

#endif