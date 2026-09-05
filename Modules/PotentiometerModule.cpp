#include "Arduino.h"
#include "PotentiometerModule.hpp"

void PotentiometerModule::begin() {
  // Nothing to do
}

void PotentiometerModule::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_POTENTIOMETER_VALUE, getDiscriminator());

  int value = analogRead(sPin);
  MessageBuilder::putU16((uint16_t)value);

  MiniCom::send(MessageBuilder::build());
}

void PotentiometerModule::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_POTENTIOMETER_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}