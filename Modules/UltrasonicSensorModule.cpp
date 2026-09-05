#include "Arduino.h"
#include "UltrasonicSensorModule.hpp"

void UltrasonicSensorModule::begin() {
  // Nothing to do
}

void UltrasonicSensorModule::update() {
  unsigned long now = millis();
  if(now - lastSent < ULTRASONICSENSOR_SEND_DELAY) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_ULTRASONICSENSOR_DISTANCE, getDiscriminator());

  long distance = sr04.Distance();
  MessageBuilder::putU16((uint16_t) distance);

  MiniCom::send(MessageBuilder::build());
}

void UltrasonicSensorModule::onMessage(Message) {
  // Nothing to handle
}