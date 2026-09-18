#ifndef _BME280MODULE_H_
#define _BME280MODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <BME280I2C.h>

#define BME280_DEFAULT_REPORTING_DELAY 1000

class BME280Module : public Module {
private:
  BME280I2C bme280;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  BME280Module(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), reportingDelay(BME280_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "bme280";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "BME280Module.cpp"
#endif

#endif