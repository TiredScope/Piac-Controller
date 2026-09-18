#include "Arduino.h"
#include "PressureSensorModule.hpp"

void PressureSensorModule::begin() {
  // Nothing to do
}

void PressureSensorModule::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_PRESSURESENSOR_VALUE, getDiscriminator());

  int value = analogRead(sPin);
  MessageBuilder::putU16((uint16_t)value);

  MiniCom::send(MessageBuilder::build());
}

void PressureSensorModule::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_PRESSURESENSOR_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}