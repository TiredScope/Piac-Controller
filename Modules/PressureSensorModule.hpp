#ifndef _PRESSURESENSORMODULE_H_
#define _PRESSURESENSORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define PRESSURESENSOR_DEFAULT_REPORTING_DELAY 500

class PressureSensorModule : public Module {
private:
  pin_t sPin;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  PressureSensorModule(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin), reportingDelay(PRESSURESENSOR_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "pressure_sensor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "PressureSensorModule.cpp"
#endif

#endif