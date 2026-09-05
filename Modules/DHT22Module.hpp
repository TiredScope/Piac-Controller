#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <DHT22.h>

#define DHT22_DEFAULT_REPORTING_DELAY 1000

class DHT22Module : public Module {
private:
  DHT22 dht22;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  DHT22Module(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), dht22(sPin), reportingDelay(DHT22_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "dht22";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "DHT22Module.cpp"
#endif

#endif