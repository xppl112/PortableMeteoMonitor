#include "ApplicationServices/UIController.h"
#include "Config.h"

UIController::UIController(HardwareRegistry* HardwareRegistry)
{
    _screen = new ScreenController(HardwareRegistry);
    _ledIndicators = new LEDIndicatorsController(HardwareRegistry);
    _soundController = new SoundController(HardwareRegistry);
    _inputsController = new InputsController(HardwareRegistry);
    _menuController = new MenuController(HardwareRegistry);

    _timer = new Ticker(UI_REDRAW_INTERVAL_MS);
    _timer->start(true);

    //Draw splash data
    showDataScreen();
    drawMainButtons();
    onNetworkStatusChange(NetworkStatus::DISABLED);
}

void UIController::updateUI() {
    updateInputs();
    if(_currentView != View::MAIN_SCREEN){
        _menuController->updateUI();
        switch(_menuController->getStatus()){
            case MenuStatus::FINISHED:
                applyMenuChanges(_menuController->getCurrentOption());               
                clearInterface();
                _currentView = View::MAIN_SCREEN;
                drawMainButtons();
                showDataScreen();
            break;
            case MenuStatus::CANCELED:
                clearInterface();
                _currentView = View::MAIN_SCREEN;                
                drawMainButtons();
                showDataScreen();
            break;
            case MenuStatus::PENDING:break;
        }
    }
    
    _timer->update();
    if(_timer->state() == FIRED){
        //redrawUI();
        _timer->start();
    }
}

void UIController::updateInputs() {
    ButtonPressed buttonPressed = _inputsController->updateInputs();

    switch(buttonPressed){
        case ButtonPressed::TOUCH:
            if(_currentView == View::MAIN_SCREEN) showMenuScreen(Menu::MODE_MENU);
            else if(_currentView == View::MODE_MENU) _menuController->selectNextOption();
            break;
        case ButtonPressed::LEFT:
            if(_currentView == View::MAIN_SCREEN) showMenuScreen(Menu::SOURCE_MENU);
            else if(_currentView == View::SOURCE_MENU) _menuController->selectNextOption();
            break;
        case ButtonPressed::CENTER:
            if(_currentView == View::MAIN_SCREEN) toggleLedEnabling();
            //else _menuController->modifyOption();
            break;
        case ButtonPressed::RIGHT:
            if(_currentView == View::MAIN_SCREEN) toggleSoundEnabling();
            else _menuController->cancel();
            break;
        case ButtonPressed::NONE:break;
    }
}

void UIController::onPresentingWeatherDataUpdate(PresentingWeatherData presentingWeatherData){
    _currentPresentingWeatherData = presentingWeatherData;
    
    if(_isLedEnabled)_ledIndicators->setWeatherStatus(_currentPresentingWeatherData);
    if(_isSoundEnabled)_soundController->setWarningLevel(_currentPresentingWeatherData.GetOverallWarningLevel());
    
    if(_currentView == View::MAIN_SCREEN){
        _screen->showDataScreen(_currentSource, _currentPresentingWeatherData);    
    }
}

void UIController::onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData presentingBackendWeatherData){
    _currentPresentingBackendWeatherData = presentingBackendWeatherData;
    
    if(_isLedEnabled)_ledIndicators->setWeatherStatus(_currentPresentingBackendWeatherData);

    if(_currentView == View::MAIN_SCREEN){
        _screen->showDataScreen(_currentSource, _currentPresentingBackendWeatherData);    
    }
}

void UIController::onNetworkStatusChange(NetworkStatus networkStatus){
    _screen->showNetworkStatusIcon(networkStatus);
}

void UIController::onBlocking(bool isBlocked){
    _isBlocked = isBlocked;
    drawMainButtons();
}

void UIController::drawMainButtons(){
    if(_currentView == View::MAIN_SCREEN)
        _screen->showMainButtons(_isBlocked, _isLedEnabled, _isSoundEnabled);
}

void UIController::clearInterface(){
    _screen->clearInterface();
}

void UIController::toggleLedEnabling(){
    _isLedEnabled = !_isLedEnabled;
    drawMainButtons();
    if(_isLedEnabled){
        _ledIndicators->setWeatherStatus(_currentPresentingWeatherData);
        _ledIndicators->setWeatherStatus(_currentPresentingBackendWeatherData);
    }
    else _ledIndicators->clearAllIndicators();
}

void UIController::toggleSoundEnabling(){
    _isSoundEnabled = !_isSoundEnabled;
    drawMainButtons();
    if(_isSoundEnabled){
        _soundController->enableAlerting(_currentPresentingWeatherData.GetOverallWarningLevel());
    }
}

void UIController::showMenuScreen(Menu menu){
    if(menu == Menu::MODE_MENU){        
        _currentView = View::MODE_MENU;
        _menuController->showMenu(menu, (uint8_t)_currentRuntimePreferences.mode);
    }
    else if(menu == Menu::SOURCE_MENU){
        _currentView = View::SOURCE_MENU;  
        _menuController->showMenu(menu, (uint8_t)_currentSource);  
    }
}

void UIController::showDataScreen(){    
    onPresentingWeatherDataUpdate(_currentPresentingWeatherData);
    onPresentingBackendWeatherDataUpdate(_currentPresentingBackendWeatherData);
}

void UIController::applyMenuChanges(uint8_t selectedOption){
    if(_currentView == View::MODE_MENU){
        if(selectedOption == (uint8_t)_currentRuntimePreferences.mode)return;
        _currentRuntimePreferences.mode = (Mode)selectedOption;
        if(_onRuntimePreferencesChangedCallback != NULL)_onRuntimePreferencesChangedCallback(_currentRuntimePreferences);
    }
    else if(_currentView == View::SOURCE_MENU){
        if(selectedOption == (uint8_t)_currentSource)return;
        _currentSource = (Source)selectedOption;
        if(_onSourceChangedCallback != NULL)_onSourceChangedCallback(_currentSource);
    }
}