#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <RTClib.h>

#define DS3231_DEFAULT_REPORTING_DELAY 1000

class DS3231Module : public Module {
private:
  RTC_DS3231 rtc;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  DS3231Module(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), reportingDelay(DS3231_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "ds3231";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "DS3231Module.cpp"
#endif

#endif