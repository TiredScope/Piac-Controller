#include "Arduino.h"
#include "PulseSensorModule.hpp"

void PulseSensorModule::begin() {
  pulseSensor.analogInput(sPin);
  pulseSensor.setThreshold(threshold);
  if (!pulseSensor.begin()) {
    MiniCom::debugPrint("Failed to initialize pulse sensor");
  }
}

void PulseSensorModule::update() {
  if (pulseSensor.sawStartOfBeat()) {
    MessageBuilder::reset(MessageType::M_PULSESENSOR_HEARTBEAT, getDiscriminator());
    MessageBuilder::putU8((uint8_t)pulseSensor.getBeatsPerMinute());
    MiniCom::send(MessageBuilder::build());
  }
}

void PulseSensorModule::onMessage(Message) {
  // Nothing to handle
}