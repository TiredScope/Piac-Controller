#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <Keypad.h>

class SparkfunKeypadModule : public Module {
private:
  Keypad keypad;
  uint16_t lastState;
public:
  SparkfunKeypadModule(const char *keymap, uint8_t *rowPins, uint8_t *colPins, uint8_t rows, uint8_t cols, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), keypad(keymap, rowPins, colPins, rows, cols){};

  virtual const char *getId() const override {
    return "sparkfun_keypad";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "SparkfunKeypadModule.cpp"
#endif

#endif