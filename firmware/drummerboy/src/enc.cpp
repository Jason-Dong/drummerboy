
#include "enc.h"
#include <Bounce2.h>

Encoder::Encoder(int8_t sw, int8_t in1, int8_t in2) {
  SW = sw;
  IN1 = in1;
  IN2 = in2;
  pinMode(IN1, INPUT_PULLUP);
  pinMode(IN2, INPUT_PULLUP);
  SWb = Bounce();
  SWb.attach(SW, INPUT_PULLUP);
  SWb.interval(25);

  val = 0;
  turn = "N/A";

  lastDir = 0;
  pinState[0] = pinState[1] = 0xff;
}

bool Encoder::fell() {
  SWb.update();
  return SWb.fell();
}

String Encoder::dir() {
  return turn;
}

int16_t Encoder::read() {

  pinState[0] = (pinState[0] << 1) | digitalReadFast(IN1);
  pinState[1] = (pinState[1] << 1) | digitalReadFast(IN2);

  int32_t i = 0;
  const uint8_t a = pinState[0] & kPinMask;
  const uint8_t b = pinState[1] & kPinMask;

  if (b == kPinEdge && a == 0x00) {
    i = 1;
    turn = "CW";
  } else if (a == kPinEdge && b == 0x00) {
    i = -1;
    turn = "CCW";
  }

  lastDir = i;

  val += i;
  return val;
}
