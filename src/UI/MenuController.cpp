#include "UI/MenuController.h"
#include "Config.h"
#include "UI/Screens/CommonDrawFunctions.h"
#include "UI/Screens/Elements/BottomButtons.h"

MenuController::MenuController(HardwareRegistry* hardwareRegistry){
    _screen = hardwareRegistry->_TFTScreen->get();
    _menuScreen = new MenuScreen(_screen);

    _timer = new Ticker(1000, NULL, MILLIS); 
}

void MenuController::showMenu(Menu menu, uint8_t selectedOption){
    _currentMenu = menu;
    _currentOption = selectedOption;
    _status = MenuStatus::PENDING;
    _currentAcceptingDelay = 0;
    redraw();
    drawBottomButtons();

    _timer->start();
}

void MenuController::redraw(bool onlyDelayPointers){
    if(onlyDelayPointers)_menuScreen->updateDelayStatus(_currentAcceptingDelay * 100 / AcceptingDelaySeconds);
    else {
        if(_currentMenu == Menu::MODE_MENU)_menuScreen->drawMenu(_modeMenuItemLabels, _modeMenuItemLabels->length(),_currentOption);
        else if(_currentMenu == Menu::SOURCE_MENU)_menuScreen->drawMenu(_sourceMenuItemLabels, _sourceMenuItemLabels->length(),_currentOption);
    }
}

void MenuController::updateUI(){
    _timer->update();
    if(_timer->state() == FIRED){
        _currentAcceptingDelay++;
        if(_currentAcceptingDelay == AcceptingDelaySeconds)_status = MenuStatus::FINISHED;
        else redraw(true);
        _timer->start();
    }
}

void MenuController::selectNextOption(){
    _currentOption++;
    if(_currentMenu == Menu::MODE_MENU && _currentOption == _modeMenuItemLabels->length())_currentOption = 0;
    else if(_currentMenu == Menu::SOURCE_MENU && _currentOption == _sourceMenuItemLabels->length())_currentOption = 0;
    
    redraw();
    _currentAcceptingDelay = 0;
    _timer->start();
}

void MenuController::modifyOption(){    
    redraw();
    _currentAcceptingDelay = 0;
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