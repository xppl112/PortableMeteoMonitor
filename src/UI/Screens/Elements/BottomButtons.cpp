#include "UI/Screens/Elements/BottomButtons.h"   
#include "UI/Screens/CommonDrawFunctions.h"   
#include <Adafruit_GFX.h> 
#include "Fonts/FreeSans9pt7b.h" 
#include "Fonts/FreeSansBold18pt7b.h" 
#include "config.h" 
    
BottomButtons::BottomButtons(Adafruit_ST7789* screen){
    _screen = screen;
}

void BottomButtons::showMainScreenButtons(bool blocked, bool isLedEnabled, bool isSoundEnabled){
    String labels[] = {
        "mode",
        "source", 
        String("led ") + (isLedEnabled ? "on" : "off"), 
        String("sound ") + (isSoundEnabled ? "on" : "off")
        };
    drawMenu(labels, blocked);
}

void BottomButtons::showModeMenuButtons(bool blocked){
    String labels[] = {"next","", "", "close"};
    drawMenu(labels, blocked);
}

void BottomButtons::showSourceMenuButtons(bool blocked){
    String labels[] = {"","next", "", "close"};
    drawMenu(labels, blocked);
}

void BottomButtons::drawMenu(String labels[], bool blocked){
    _screen->drawLine(0, 225, 320, 225, MainButtonsDividerColor);
    uint16_t buttonBackgroundColor = blocked ? MainButtonsDisabledBackground : MainButtonsBackground;
    uint16_t buttonTextColor = blocked ? MainButtonsDisabledTextColor : MainButtonsDefaultTextColor;

    _screen->fillRect(15, 228, 64, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, labels[0], 6, buttonTextColor,15,79,230);

    _screen->fillRect(81, 228, 78, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, labels[1], 6, buttonTextColor,81,159,230);

    _screen->fillRect(161, 228, 78, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, labels[2], 6, buttonTextColor,161,239,230);

    _screen->fillRect(241, 228, 79, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, labels[3], 6, buttonTextColor,241,320,230);
}