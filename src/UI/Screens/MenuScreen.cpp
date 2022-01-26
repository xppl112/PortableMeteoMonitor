#include "UI/Screens/MenuScreen.h"
#include <Adafruit_GFX.h>
#include "UI/Screens/CommonDrawFunctions.h"   

MenuScreen::MenuScreen(Adafruit_ST7789* screen){
    _screen = screen;    
}

void MenuScreen::drawMenu(String options[], uint8_t optionsCount, uint8_t selectedOption){
    _selectedOption = selectedOption;
    _screen->fillRect(30,0,260,OptionHeight * optionsCount, BackgroundColor);
    _screen->drawRect(30,0,261,OptionHeight * optionsCount+1, ST77XX_BLACK);
    _screen->drawRect(30,0,260,OptionHeight * optionsCount, ST77XX_WHITE);
    
}

void MenuScreen::updateDelayStatus(uint8_t percentDone){
   
}
