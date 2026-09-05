#include "Arduino.h"
#include "DS3231Module.hpp"

void DS3231Module::begin() {
  Wire.begin();

  if (!rtc.begin()) {
    MiniCom::debugPrint("Failed to initialize DS3231");
  }
}

void DS3231Module::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_DS3231_VALUES, getDiscriminator());

  MessageBuilder::putU32(rtc.now().unixtime());

  MiniCom::send(MessageBuilder::build());
}

void DS3231Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_DS3231_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
    case MessageType::M_DS3231_SET_TIME:
      {
        uint32_t time = m.getU32(idx);
        rtc.adjust(DateTime(time));
        break;
      }
    default:
      {
        break;
      }
  }
}