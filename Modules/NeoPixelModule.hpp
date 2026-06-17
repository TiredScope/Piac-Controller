#ifndef _LEDMATRIXMODULE_H_
#define _LEDMATRIXMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"
#include <Adafruit_NeoPixel.h>

class NeoPixelModule : public Module {
private:
  Adafruit_NeoPixel pixels;
public:
  NeoPixelModule(uint16_t ledCount, pin_t dPin, neoPixelType type = NEO_GRB + NEO_KHZ800, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), pixels(ledCount, dPin, type){};

  virtual const char *getId() const override {
    return "neopixel";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "NeoPixelModule.cpp"
#endif

#endif