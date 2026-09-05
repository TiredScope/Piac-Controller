#include "Arduino.h"
#include "ThermistorModule.hpp"

void ThermistorModule::begin() {
  // Nothing to do
}

void ThermistorModule::update() {
  unsigned long now = millis();
  if (now - lastSent < THERMISTOR_SEND_DELAY) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_THERMISTOR_VALUE, getDiscriminator());

  int value = analogRead(sPin);
  MessageBuilder::putU16((uint16_t)value);

  MiniCom::send(MessageBuilder::build());
}

void ThermistorModule::onMessage(Message) {
  // Nothing to handle
}