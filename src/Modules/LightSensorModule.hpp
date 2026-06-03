#ifndef _PULSESENSORMODULE_H_
#define _PULSESENSORMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"
#include "../HC-SR04/SR04.h"

#define LIGHTSENSOR_SEND_DELAY 100

class LightSensorModule : public Module {
private:
  pin_t sPin;
  unsigned long lastSent;
public:
  LightSensorModule(pin_t sPin){};

  virtual const char *getId() const override {
    return "light_sensor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif