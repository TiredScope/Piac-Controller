#ifndef _LEDMATRIXMODULE_H_
#define _LEDMATRIXMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"
#include <LedControl.h>

#define LEDMATRIX_RESOLUTION_X 8
#define LEDMATRIX_RESOLUTION_Y 8

class LEDMatrixModule : public Module {
private:
  LedControl control;
public:
  LEDMatrixModule(pin_t dInPin, pin_t clkPin, pin_t csPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), control(dInPin, clkPin, csPin){};

  virtual const char *getId() const override {
    return "led_matrix";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif