#pragma once

#include <Adafruit_ST7789.h> 

class MenuScreen
{
public:
    MenuScreen(Adafruit_ST7789* screen);

    void drawMenu(String title, String options[], uint8_t optionsCount, uint8_t selectedOption);
    void updateDelayStatus(uint8_t selectedOption, uint8_t percentDone);
    void redrawOption(uint8_t option, String label, bool isSelected);

private:
    Adafruit_ST7789* _screen;

    static const uint16_t BorderColor = 0xB596;
    static const uint16_t BackgroundColor = 0x18E3;
    static const uint16_t TextColor = 0xFFFF;
    static const uint16_t SelectedOptionBackgroundColor = 0xBDD7;  
    static const uint16_t SelectedOptionTextColor = 0x0000;
    static const uint16_t DelayLineColor = 0xFFFF;

    static const uint16_t OptionHeight = 25;
    static const uint16_t TitleHeight = 29;
    static const uint16_t X = 30;
    static const uint16_t Y = 0;
    static const uint16_t OptionsY = Y+TitleHeight+8;
    static const uint16_t Width = 260;
};