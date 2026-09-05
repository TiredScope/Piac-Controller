#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <SensirionI2cScd4x.h>

#define SCD41_DEFAULT_REPORTING_DELAY 1000

class SCD41Module : public Module {
private:
  SensirionI2cScd4x scd41;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  SCD41Module(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), reportingDelay(SCD41_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "scd41";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "SCD41Module.cpp"
#endif

#endif