#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <Adafruit_BMP280.h>

#define BMP280_DEFAULT_REPORTING_DELAY 1000

class BMP280Module : public Module {
private:
  Adafruit_BMP280 bmp280;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  BMP280Module(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), reportingDelay(BMP280_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "bmp280";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "BMP280Module.cpp"
#endif

#endif