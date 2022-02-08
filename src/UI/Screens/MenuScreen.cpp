#include "UI/Screens/MenuScreen.h"
#include <Adafruit_GFX.h>
#include "UI/Screens/CommonDrawFunctions.h"   

MenuScreen::MenuScreen(Adafruit_ST7789* screen){
    _screen = screen;    
}

void MenuScreen::drawMenu(String title, String options[], uint8_t optionsCount, uint8_t selectedOption){
    uint16_t OverallHeight = OptionsY + OptionHeight * optionsCount;

    _screen->drawRect(X-1,Y,Width+2,OverallHeight+1, ST77XX_BLACK);
    _screen->fillRect(X,Y,Width,TitleHeight, BackgroundColor);
    _screen->drawLine(X,TitleHeight,X+Width,TitleHeight, BorderColor);
    CommonDrawFunctions::drawCenteredText(_screen, title, 9, TextColor, X, X+Width, Y+20);

    _screen->fillRect(X,TitleHeight+1,Width,OverallHeight - TitleHeight-1, BackgroundColor);
    _screen->drawRect(X,Y,Width,OverallHeight, BorderColor);

    for(uint8_t i = 0; i < optionsCount; i++){
        if(selectedOption == i){
            _screen->fillRect(X+1,OptionsY + i * OptionHeight,Width-2,OptionHeight-1, SelectedOptionBackgroundColor);
        }

        CommonDrawFunctions::drawText(_screen, options[i], 9, selectedOption == i ? SelectedOptionTextColor : TextColor, X+6, OptionsY + i * OptionHeight + 15);
    }
}

void MenuScreen::redrawOption(uint8_t option, String label, bool isSelected){
    _screen->fillRect(X+1,OptionsY + option * OptionHeight,Width-2,OptionHeight-1, isSelected ? SelectedOptionBackgroundColor : BackgroundColor);
    CommonDrawFunctions::drawText(_screen, label, 9, isSelected ? SelectedOptionTextColor : TextColor, X+6, OptionsY + option * OptionHeight + 15);
}

void MenuScreen::updateDelayStatus(uint8_t selectedOption, uint8_t percentDone){    
   _screen->fillRect(X+1,OptionsY + selectedOption * OptionHeight + OptionHeight - 4,percentDone * (Width-2) / 100,3, DelayLineColor);
}
