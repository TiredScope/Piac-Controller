#include "Arduino.h"
#include "LightSensorModule.hpp"

void LightSensorModule::begin() {
  // Nothing to do
}

void LightSensorModule::update() {
  unsigned long now = millis();
  if (now - lastSent < LIGHTSENSOR_SEND_DELAY) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_LIGHTSENSOR_VALUE, getDiscriminator());

  int value = analogRead(sPin);
  MessageBuilder::putU16((uint16_t)value);

  MiniCom::send(MessageBuilder::build());
}

void LightSensorModule::onMessage(Message) {
  // Nothing to handle
}