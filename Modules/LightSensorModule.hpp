#ifndef _LIGHTSENSORMODULE_H_
#define _LIGHTSENSORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define LIGHTSENSOR_SEND_DELAY 100

class LightSensorModule : public Module {
private:
  pin_t sPin;
  unsigned long lastSent;
public:
  LightSensorModule(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin){};

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