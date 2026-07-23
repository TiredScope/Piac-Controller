#ifndef _LIGHTSENSORMODULE_H_
#define _LIGHTSENSORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define LIGHTSENSOR_DEFAULT_REPORTING_DELAY 500

class LightSensorModule : public Module {
private:
  pin_t sPin;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  LightSensorModule(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin), reportingDelay(LIGHTSENSOR_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "light_sensor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "LightSensorModule.cpp"
#endif

#endif