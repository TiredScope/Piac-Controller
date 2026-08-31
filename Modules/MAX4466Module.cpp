#include "Arduino.h"
#include "MAX4466Module.hpp"

void MAX4466Module::begin() {
  // Nothing to do
}

void MAX4466Module::update() {
  unsigned long now = millis();

  int value = analogRead(sPin);
  if (value > maxSample) {
    maxSample = value;
  }

  if (value < minSample) {
    minSample = value;
  }

  if (now - lastSent < reportingDelay || maxSample <= minSample) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_MAX4466_VALUE, getDiscriminator());

  MessageBuilder::putU16((uint16_t)(maxSample - minSample));
  maxSample = 0;
  minSample = 0xFFFF;

  MiniCom::send(MessageBuilder::build());
}

void MAX4466Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_MAX4466_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
    default:
      {
      }
  }
}