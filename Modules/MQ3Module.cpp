#include "Arduino.h"
#include "MQ3Module.hpp"

void MQ3Module::begin() {
  // Nothing to do
  lastState = 0xFF;
}

void MQ3Module::update() {
  bool alcoholDetected = digitalRead(dPin) != HIGH;

  if (aPin != INVALID_PIN && reportingDelay > 0) {
    unsigned long now = millis();
    if (now - lastSent < reportingDelay) return;
    lastSent = now;

    MessageBuilder::reset(MessageType::M_MQ3_VALUES, getDiscriminator());

    MessageBuilder::putU8(alcoholDetected);
    MessageBuilder::putU16(analogRead(aPin));

    MiniCom::send(MessageBuilder::build());
  }

  if (alcoholDetected != lastState) {
    lastState = alcoholDetected;

    MessageBuilder::reset(MessageType::M_MQ3_STATE_CHANGED, getDiscriminator());

    MessageBuilder::putU8(alcoholDetected);

    MiniCom::send(MessageBuilder::build());
  }
}

void MQ3Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_MQ3_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}