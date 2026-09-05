#ifndef _LCDMODULE_H_
#define _LCDMODULE_H_

#include <LiquidCrystal.h>
#include <Arduino.h>
#include "../Module.hpp"
#include "../Common.hpp"

// LCD config flags
#define LCD_CURSOR 0x01
#define LCD_BLINK 0x02
#define LCD_AUTOSCROLL 0x04
#define LCD_DISPLAY 0x08
#define LCD_RIGHT_TO_LEFT 0x10  // 1 == scroll left, 0 == scroll right

class LCDModule : public Module {
private:
  LiquidCrystal lcd;
public:
  LCDModule(pin_t rs, pin_t en, pin_t d4, pin_t d5, pin_t d6, pin_t d7, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), lcd(rs, en, d4, d5, d6, d7){};

  virtual const char *getId() const override {
    return "lcd";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "LCDModule.cpp"
#endif

#endif