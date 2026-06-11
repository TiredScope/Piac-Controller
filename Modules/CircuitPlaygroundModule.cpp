#include "Adafruit_Circuit_Playground.h"
#include "Arduino.h"
#include "CircuitPlaygroundModule.hpp"
#include "Adafruit_CircuitPlayground.h"

void CircuitPlaygroundModule::begin() {
}

void CircuitPlaygroundModule::update() {
  // Received something?
  //MessageBuilder::reset(MessageType::M_IRRECEIVER_DATA, getDiscriminator());
  //MessageBuilder::putU16((uint16_t)data.protocol);
  //MessageBuilder::putU8(data.flags);
  //MessageBuilder::putU16(data.address);
  //MessageBuilder::putU16(data.command);
  //MessageBuilder::putU16(data.extra);
  //MiniCom::send(MessageBuilder::build());
}

void CircuitPlaygroundModule::onMessage(Message m) {
  // Nothing to handle
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

        if (target & CIRCUITPLAYGROUND_BUTTONS) {
          // Buttons are not sent continuously
        }

        if (target & CIRCUITPLAYGROUND_SLIDE_SWITCH) {
          // Switch is not sent continuously
        }

        if (target & CIRCUITPLAYGROUND_CAPTOUCH) {
          capTouchDelay = delay;
        }

        if (target & CIRCUITPLAYGROUND_ACCELEROMETER) {
          accelerometerDelay = delay;
        }

        if (target & CIRCUITPLAYGROUND_TEMPERATURE) {
          temperatureDelay = delay;
        }

        if (target & CIRCUITPLAYGROUND_LIGHT) {
          lightDelay = delay;
        }

        if (target & CIRCUITPLAYGROUND_SOUND) {
          soundDelay = delay;
        }

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
      /*case M_CIRCUITPLAYGROUND_BUTTONS:
      {
        break;
      }
    case M_CIRCUITPLAYGROUND_CAPTOUCH:
      {
        break;
      }
    case M_CIRCUITPLAYGROUND_ACCELEROMETER:
      {
        break;
      }
    case M_CIRCUITPLAYGROUND_TEMPERATURE:
      {
        break;
      }
    case M_CIRCUITPLAYGROUND_LIGHT:
      {
        break;
      }
    case M_CIRCUITPLAYGROUND_SOUND:
      {
        break;
      }*/
  }
}