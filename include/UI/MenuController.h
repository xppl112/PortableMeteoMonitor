#pragma once

#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/TFTScreen.h"
#include "UI/Screens/MenuScreen.h"
#include <Ticker.h>

enum class Menu { MODE_MENU, SOURCE_MENU };
enum class MenuStatus { PENDING, FINISHED, CANCELED };

class MenuController
{
public:
    MenuController(HardwareRegistry* hardwareRegistry);
    void showMenu(Menu menu, uint8_t selectedOption);
    MenuStatus getStatus() {return _status;}
    uint8_t getCurrentOption() {return _currentOption;}
    void updateUI();
    void redraw();
    void redrawOption(uint8_t option, bool isSelected);
    void redrawDelayPointers();

    void selectNextOption();
    void modifyOption();
    void cancel();

private:
    Adafruit_ST7789* _screen;
    MenuScreen* _menuScreen;
    Ticker* _timer;

    Menu _currentMenu;
    volatile MenuStatus _status = MenuStatus::PENDING;
    uint8_t _currentOption;
    
    void drawBottomButtons();

    String _modeMenuItemLabels[3]{"Standard mode","Active monitoring", "Night mode"};
    String _sourceMenuItemLabels[4]{"Mixed data","Local data","Out weather","Solar station"};
    const uint16_t AcceptingDelayStepMs = 25;
    const uint8_t AcceptingDelaySteps = 80;
    uint8_t _currentAcceptingDelayStep = 0;
};