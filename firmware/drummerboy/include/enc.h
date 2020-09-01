#ifndef ENC_H
#define ENC_H

#include <Bounce2.h>

class Encoder {

  public:
    Encoder(int8_t, int8_t, int8_t);
    bool fell();
    String dir();
    int16_t read();

  private:
    int8_t SW, IN1, IN2;
    Bounce SWb;
    int32_t val;
    String turn;
    static const uint8_t kPinMask = 0x03;
    static const uint8_t kPinEdge = 0x02;

    int32_t lastDir;
    uint8_t pinState[2];
};


#endif //ENC_H
