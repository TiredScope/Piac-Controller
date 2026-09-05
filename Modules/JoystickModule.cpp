#include "Arduino.h"
#include "JoystickModule.hpp"

void JoystickModule::begin() {
  pinMode(swPin, INPUT);
}

void JoystickModule::update() {
  unsigned long now = millis();
  if(now - lastSent < JOYSTICK_SEND_DELAY) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_JOYSTICK_VALUES, getDiscriminator());

  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int sw = digitalRead(swPin);
  MessageBuilder::putF32(x);
  MessageBuilder::putF32(y);
  MessageBuilder::putU8(sw);

  MiniCom::send(MessageBuilder::build());
}

void JoystickModule::onMessage(Message) {
  // Nothing to handle
}