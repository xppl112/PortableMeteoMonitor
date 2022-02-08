#include "UI/MenuController.h"
#include "Config.h"
#include "UI/Screens/CommonDrawFunctions.h"
#include "UI/Screens/Elements/BottomButtons.h"

MenuController::MenuController(HardwareRegistry* hardwareRegistry){
    _screen = hardwareRegistry->_TFTScreen->get();
    _menuScreen = new MenuScreen(_screen);

    _timer = new Ticker(AcceptingDelayStepMs, NULL, MILLIS); 
}

void MenuController::showMenu(Menu menu, uint8_t selectedOption){
    _currentMenu = menu;
    _currentOption = selectedOption;
    _status = MenuStatus::PENDING;
    _currentAcceptingDelayStep = 0;
    drawBottomButtons();
    redraw();    

    _timer->start();
}

void MenuController::redraw(){
    if(_currentMenu == Menu::MODE_MENU)_menuScreen->drawMenu("Mode:", _modeMenuItemLabels, 3,_currentOption);
    else if(_currentMenu == Menu::SOURCE_MENU)_menuScreen->drawMenu("Source:", _sourceMenuItemLabels, 4,_currentOption);
}

void MenuController::redrawOption(uint8_t option, bool isSelected){
    if(_currentMenu == Menu::MODE_MENU)_menuScreen->redrawOption(option, _modeMenuItemLabels[option], isSelected);
    else if(_currentMenu == Menu::SOURCE_MENU)_menuScreen->redrawOption(option, _sourceMenuItemLabels[option], isSelected);    
}

void MenuController::redrawDelayPointers(){
    _menuScreen->updateDelayStatus(_currentOption, _currentAcceptingDelayStep * 100 / AcceptingDelaySteps);
}

void MenuController::updateUI(){
    _timer->update();
    if(_timer->state() == FIRED){
        _currentAcceptingDelayStep++;
        if(_currentAcceptingDelayStep == AcceptingDelaySteps)_status = MenuStatus::FINISHED;
        redrawDelayPointers();
        _timer->start();
    }
}

void MenuController::selectNextOption(){
    redrawOption(_currentOption, false);
    _currentOption++;
    if(_currentMenu == Menu::MODE_MENU && _currentOption == 3)_currentOption = 0;
    else if(_currentMenu == Menu::SOURCE_MENU && _currentOption == 4)_currentOption = 0; 
    redrawOption(_currentOption, true);

    _currentAcceptingDelayStep = 0;
    _timer->start();
}

void MenuController::modifyOption(){    
    redrawOption(_currentOption, true);
    _currentAcceptingDelayStep = 0;
    _timer->start();
}

void MenuController::cancel(){
    _status = MenuStatus::CANCELED;
}

void MenuController::drawBottomButtons(){
    BottomButtons bottomButtons(_screen);
    if(_currentMenu == Menu::MODE_MENU)bottomButtons.showModeMenuButtons(false);
    else if(_currentMenu == Menu::SOURCE_MENU)bottomButtons.showSourceMenuButtons(false);
}