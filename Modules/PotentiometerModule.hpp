#ifndef _POTENTIOMETERMODULE_H_
#define _POTENTIOMETERMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define POTENTIOMETER_DEFAULT_REPORTING_DELAY 500

class PotentiometerModule : public Module {
private:
  pin_t sPin;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  PotentiometerModule(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin), reportingDelay(POTENTIOMETER_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "potentiometer";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "PotentiometerModule.cpp"
#endif

#endif