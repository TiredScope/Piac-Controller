#include "LCDModule.hpp"

#include <LiquidCrystal.h>

void LCDModule::begin() {
  lcd.begin(16, 2);  // TODO: hard-coded values
  lcd.print("OK!");
}

void LCDModule::update() {
}

void LCDModule::onMessage(Message m) {
  switch (m.getType()) {
    case MessageType::M_LCD_SETTINGS:
      {
        size_t idx = 0;
        uint32_t flags = m.getU32(idx);
        if (flags & LCD_CURSOR) {
          lcd.cursor();
        } else {
          lcd.noCursor();
        }

        if (flags & LCD_BLINK) {
          lcd.blink();
        } else {
          lcd.noBlink();
        }

        if (flags & LCD_AUTOSCROLL) {
          lcd.autoscroll();
        } else {
          lcd.noAutoscroll();
        }

        if (flags & LCD_DISPLAY) {
          lcd.display();
        } else {
          lcd.noDisplay();
        }

        if (flags & LCD_RIGHT_TO_LEFT) {
          lcd.rightToLeft();
        } else {
          lcd.leftToRight();
        }

        // TODO: can be refactored and made more efficient using lcd.command(...) with the appropriate bits set
        break;
      }
    case MessageType::M_LCD_CLEAR:
      {
        lcd.clear();
        break;
      }
    case MessageType::M_LCD_PRINT:
      {
        size_t idx = 0;
        const char *message = m.getString(idx);
        if (!message) return;
        lcd.print(message);
      }
    default: return;
  }
}