#include "Adafruit_LIS3DH.h"
#include "Arduino.h"
#include "LIS3DHModule.hpp"

#include <Wire.h>

void LIS3DHModule::begin() {
  Wire.begin();

  if (!lis.begin(0x18)) {
    MiniCom::debugPrint("Failed to connect to LIS3DH");
  }
}

void LIS3DHModule::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  lis.read();

  sensors_event_t event;
  lis.getEvent(&event);

  MessageBuilder::reset(MessageType::M_LIS3DH_VALUES, getDiscriminator());

  MessageBuilder::putF32(event.acceleration.x);
  MessageBuilder::putF32(event.acceleration.y);
  MessageBuilder::putF32(event.acceleration.z);

  MiniCom::send(MessageBuilder::build());
}

void LIS3DHModule::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_LIS3DH_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
    case MessageType::M_LIS3DH_SET_PARAMS:
      {
        uint8_t dataRate = m.getU8(idx);
        uint8_t performanceMode = m.getU8(idx);
        uint8_t range = m.getU8(idx);

        lis.setDataRate((lis3dh_dataRate_t)dataRate);
        lis.setPerformanceMode((lis3dh_mode_t)performanceMode);
        lis.setRange((lis3dh_range_t)range);
        break;
      }
    default:
      {
        break;
      }
  }
}