#ifndef _MAX4466MODULE_H_
#define _MAX4466MODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define MAX4466_DEFAULT_REPORTING_DELAY 100
#define MAX4466_SAMPLE_DELAY 50 // 50ms = 20hz minimum detectable frequency

class MAX4466Module : public Module {
private:
  pin_t sPin;
  uint32_t reportingDelay;
  unsigned long lastSent;

  uint16_t minSample;
  uint16_t maxSample;
public:
  MAX4466Module(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin), reportingDelay(MAX4466_DEFAULT_REPORTING_DELAY), minSample(0xFFFF), maxSample(0x0000){};

  virtual const char *getId() const override {
    return "max4466";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "MAX4466Module.cpp"
#endif

#endif