#include "Arduino.h"
#include "NeoPixelModule.hpp"

void NeoPixelModule::begin() {
  pixels.begin();
}

void NeoPixelModule::update() {
}

void NeoPixelModule::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_NEOPIXEL_SET_BRIGHTNESS:
      {
        uint8_t brightness = m.getU8(idx);
        pixels.setBrightness(brightness);
        pixels.show();
        break;
      }
    case MessageType::M_NEOPIXEL_SET_COLORS:
      {
        uint16_t offset = m.getU16(idx);

        uint16_t i = offset;
        while (idx < m.getLength()) {
          uint8_t r = m.getU8(idx);
          uint8_t g = m.getU8(idx);
          uint8_t b = m.getU8(idx);
          uint8_t w = m.getU8(idx);

          pixels.setPixelColor(i, r, g, b, w);

          i++;
        }

        pixels.show();

        break;
      }
    default: return;
  }
}