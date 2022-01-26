#pragma once

#include <Adafruit_ST7789.h>

class BottomButtons
{
public:
    BottomButtons(Adafruit_ST7789* screen);

    void showMainScreenButtons(bool blocked, bool isLedEnabled, bool isSoundEnabled);
    void showModeMenuButtons(bool blocked);
    void showSourceMenuButtons(bool blocked);
    
private:
    Adafruit_ST7789* _screen;
    void drawMenu(String labels[], bool blocked);

    static const uint16_t MainButtonsDividerColor = 0x3186;
    static const uint16_t MainButtonsBackground = 0x3186;
    static const uint16_t MainButtonsDefaultTextColor = 0xAD75;
    static const uint16_t MainButtonsDisabledBackground = 0x58E3;
    static const uint16_t MainButtonsDisabledTextColor = 0x8AEB;
};
