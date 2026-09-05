#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define MQ3_DEFAULT_REPORTING_DELAY 1000

class MQ3Module : public Module {
private:
  pin_t dPin;
  pin_t aPin;
  uint32_t reportingDelay;
  unsigned long lastSent;
  uint8_t lastState;
public:
  MQ3Module(pin_t dPin, pin_t aPin = INVALID_PIN, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), dPin(dPin), aPin(aPin), reportingDelay(MQ3_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "mq3";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "MQ3Module.cpp"
#endif

#endif