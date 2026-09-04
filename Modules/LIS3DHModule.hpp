#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#define LIS3DH_DEFAULT_REPORTING_DELAY 1000

class LIS3DHModule : public Module {
private:
  Adafruit_LIS3DH lis;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  LIS3DHModule(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), reportingDelay(LIS3DH_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "lis3dh";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "LIS3DHModule.cpp"
#endif

#endif