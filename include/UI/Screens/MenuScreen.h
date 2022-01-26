#pragma once

#include <Adafruit_ST7789.h> 

class MenuScreen
{
public:
    MenuScreen(Adafruit_ST7789* screen);

    void drawMenu(String options[], uint8_t optionsCount, uint8_t selectedOption);
    void updateDelayStatus(uint8_t percentDone);

private:
    Adafruit_ST7789* _screen;
    uint8_t _selectedOption;

    static const uint16_t BackgroundColor = 0x18E3;
    static const uint16_t TextColor = 0xFFFF;
    static const uint16_t SelectedOptionBackgroundColor = 0xC638;  
    static const uint16_t SelectedOptionTextColor = 0x0000;

    static const uint16_t DelayLineColor = 0x0640;
    static const uint16_t OptionHeight = 30;
};