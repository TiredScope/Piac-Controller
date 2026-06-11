#include "Adafruit_Circuit_Playground.h"
#include "Arduino.h"
#include "CircuitPlaygroundModule.hpp"
#include "Adafruit_CircuitPlayground.h"

bool CircuitPlaygroundModule::needsToBeSent(CircuitPlaygroundComponent component) {
  if (!(reportingMode & CIRCUITPLAYGROUND_COMPONENT_FLAG(component))) {
    // Component is disabled
    return false;
  }

  bool toBeSent = millis() - lastSent[(uint8_t)component] >= delays[(uint8_t)component];
  if (toBeSent) {
    lastSent[(uint8_t)component] = millis();
  }

  return toBeSent;
}

void CircuitPlaygroundModule::begin() {
  for (uint8_t i = 0; i < (uint8_t)CircuitPlaygroundComponent::MAX_VALUE; i++) {
    delays[i] = CIRCUITPLAYGROUND_DEFAULT_DELAY;
  }

  // Ensure the button state is sent when the module is first activated
  lastSent[(uint8_t)CircuitPlaygroundComponent::BUTTONS] = 0xFF;

  CircuitPlayground.begin();
}

void CircuitPlaygroundModule::update() {
  uint8_t buttonState = (CircuitPlayground.slideSwitch() << 2) | (CircuitPlayground.leftButton() << 1) | CircuitPlayground.rightButton();
  // We kind of abuse lastSent here to store the previous state, but since it is unused otherwise anyway, why not?
  if ((reportingMode & CIRCUITPLAYGROUND_COMPONENT_FLAG(CircuitPlaygroundComponent::BUTTONS)) && buttonState != lastSent[(uint8_t)CircuitPlaygroundComponent::BUTTONS]) {
    lastSent[(uint8_t)CircuitPlaygroundComponent::BUTTONS] = buttonState;

    MessageBuilder::reset(MessageType::M_CIRCUITPLAYGROUND_BUTTONS, getDiscriminator());
    MessageBuilder::putU8(buttonState);
    MiniCom::send(MessageBuilder::build());
  }

  if (needsToBeSent(CircuitPlaygroundComponent::CAPACITIVE_TOUCH)) {
    MessageBuilder::reset(MessageType::M_CIRCUITPLAYGROUND_CAPACITIVE_TOUCH, getDiscriminator());

    MessageBuilder::putU8(capacitiveTouchPins);

    // TODO: Currently, we do not allow setting numSamples and just use the default of 10
    for (uint8_t pin = 0; pin < 8; pin++) {
      if (!(capacitiveTouchPins & (1 << pin))) {
        continue;
      }

      uint16_t cap = CircuitPlayground.readCap(CIRCUITPLAYGROUND_CAPACITIVE_TOUCH_PINS[pin]);
      MessageBuilder::putU16(cap);
    }

    MiniCom::send(MessageBuilder::build());
  }

  if (needsToBeSent(CircuitPlaygroundComponent::ACCELEROMETER)) {
    MessageBuilder::reset(MessageType::M_CIRCUITPLAYGROUND_ACCELEROMETER, getDiscriminator());

    MessageBuilder::putF32(CircuitPlayground.motionX());
    MessageBuilder::putF32(CircuitPlayground.motionY());
    MessageBuilder::putF32(CircuitPlayground.motionZ());

    MiniCom::send(MessageBuilder::build());
  }

  if (needsToBeSent(CircuitPlaygroundComponent::TEMPERATURE)) {
    MessageBuilder::reset(MessageType::M_CIRCUITPLAYGROUND_TEMPERATURE, getDiscriminator());

    MessageBuilder::putF32(CircuitPlayground.temperature());

    MiniCom::send(MessageBuilder::build());
  }

  if (needsToBeSent(CircuitPlaygroundComponent::LIGHT)) {
    MessageBuilder::reset(MessageType::M_CIRCUITPLAYGROUND_LIGHT, getDiscriminator());

    MessageBuilder::putU16(CircuitPlayground.lightSensor());

    MiniCom::send(MessageBuilder::build());
  }

  if (needsToBeSent(CircuitPlaygroundComponent::SOUND)) {
    MessageBuilder::reset(MessageType::M_CIRCUITPLAYGROUND_SOUND, getDiscriminator());

    MessageBuilder::putI16(CircuitPlayground.soundSensor());

    MiniCom::send(MessageBuilder::build());
  }
}

void CircuitPlaygroundModule::onMessage(Message m) {
  size_t idx = 0;
  switch (m.getType()) {
    case MessageType::M_CIRCUITPLAYGROUND_SET_REPORTING_MODE:
      {
        uint8_t reportingMode = m.getU8(idx);
        this->reportingMode = reportingMode;
        break;
      }
    case MessageType::M_CIRCUITPLAYGROUND_SET_REPORTING_DELAY:
      {
        uint8_t target = m.getU8(idx);
        uint32_t delay = m.getU32(idx);

        // Technically, like this, delays can also be set for the buttons component, but this makes it easier to expand and more readable
        for (uint8_t component = (uint8_t)CircuitPlaygroundComponent::BUTTONS; component < (uint8_t)CircuitPlaygroundComponent::MAX_VALUE; component++) {
          if (target & CIRCUITPLAYGROUND_COMPONENT_FLAG(component)) {
            delays[component] = delay;
          }
        }

        break;
      }
    case MessageType::M_CIRCUITPLAYGROUND_SET_CAPACITIVE_TOUCH_PINS:
      {
        uint8_t pins = m.getU8(idx);
        capacitiveTouchPins = pins;
        break;
      }
    case MessageType::M_CIRCUITPLAYGROUND_SET_PIXEL:
      {
        uint8_t pixel = m.getU8(idx);
        uint8_t r = m.getU8(idx);
        uint8_t g = m.getU8(idx);
        uint8_t b = m.getU8(idx);
        CircuitPlayground.setPixelColor(pixel, r, g, b);
        break;
      }
    case MessageType::M_CIRCUITPLAYGROUND_SET_BRIGHTNESS:
      {
        uint16_t brightness = m.getU8(idx);
        CircuitPlayground.setBrightness(brightness);
        break;
      }
    default:
      {
        break;
      }
  }
}