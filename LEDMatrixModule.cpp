#include "Arduino.h"
#include "LEDMatrixModule.hpp"
#include "MiniCom.hpp"

void LEDMatrixModule::begin() {
}

void LEDMatrixModule::update() {
  // Nothing to update
}

void LEDMatrixModule::onMessage(Message m) {
  switch (m.getType()) {
    case MessageType::M_LEDMATRIX_IMAGE:
      {
        size_t idx = 0;
        for (uint8_t row = 0; row < 8; row++) {
          control.setRow(0, row, m.getU8(idx));
        }
        break;
      }
    default: return;
  }
}