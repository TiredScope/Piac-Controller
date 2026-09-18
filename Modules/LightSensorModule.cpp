#include "Arduino.h"
#include "LightSensorModule.hpp"

void LightSensorModule::begin() {
  // Nothing to do
}

void LightSensorModule::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_LIGHTSENSOR_VALUE, getDiscriminator());

  int value = analogRead(sPin);
  MessageBuilder::putU16((uint16_t)value);

  MiniCom::send(MessageBuilder::build());
}

void LightSensorModule::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_LIGHTSENSOR_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}