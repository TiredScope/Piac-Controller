#include "Arduino.h"
#include "PIRSensorModule.hpp"

void PIRSensorModule::begin() {
  pinMode(sPin, INPUT);
}

void PIRSensorModule::update() {
  unsigned long now = millis();
  if(now - lastSent < PIRSENSOR_SEND_DELAY) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_PIRSENSOR_PRESENCE, getDiscriminator());

  int presence = digitalRead(sPin);
  MessageBuilder::putU8((uint8_t) presence);

  MiniCom::send(MessageBuilder::build());
}

void PIRSensorModule::onMessage(Message) {
  // Nothing to handle
}