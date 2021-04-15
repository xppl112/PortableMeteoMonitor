#include "ApplicationServices/UIController.h"
#include "GlobalObjects/GlobalSystemState.h"
#include "Config.h"
extern GlobalSystemState* globalSystemState;

UIController::UIController(
    HardwareRegistry* HardwareRegistry, 
    HealthcheckController* healthcheckController)
{
    _screen = new ScreenController(HardwareRegistry);
    _ledDisplayController = new LEDDisplayController(HardwareRegistry);
    _menuController = new MenuController(HardwareRegistry, healthcheckController);
    _ledIndicators = new LEDIndicatorsController(HardwareRegistry);
    _inputsController = new InputsController(HardwareRegistry);

    _screen->showSplashScreen();
}

void UIController::updateUI() {
    updateOutputDevicesStatus(); 
    updateInputs();
    
    _ledIndicators->updateSystemStatusLed();
    if(_isMenuMode){
        _menuController->refresh();
    }
}

void UIController::updateInputs() {
    ButtonPressed buttonPressed = _inputsController->updateInputs();

    if(_isMenuMode){
        _menuController->buttonPressed(buttonPressed);
        if(!_menuController->isMenuActive()){
            disableMenuMode();
        }
    }
    else if(_isScreenActive){
        switch(buttonPressed){
            case ButtonPressed::LEFT:
                flipScreenMode(false);
                redrawUI();
                break;
            case ButtonPressed::RIGHT:
                flipScreenMode(true);
                redrawUI();
                break;
            case ButtonPressed::UP:
                enableMenuMode();
                break;
            case ButtonPressed::DOWN:
                enableMenuMode();
                break;
            case ButtonPressed::LEFTRIGHT:
                enableMenuMode(true);
                break;
            case ButtonPressed::NONE:break;
        }
    }

    if(buttonPressed != ButtonPressed::NONE){
        onInteraction();
        redrawUI();
    }
}

void UIController::onWeatherUpdated(WeatherMonitorData weatherMonitorData){
    _currentWeather = weatherMonitorData;
    if(!globalSystemState->isNightMode){
        onInteraction();
    }

    redrawUI();
}

void UIController::redrawUI(){
    if(globalSystemState->isNightMode && !_isInteraction){
        _ledIndicators->clearAllIndicators();
    }
    else {
        _ledIndicators->setPollutionLevel(_currentWeather);
        _ledIndicators->setWeatherStatusLed(_currentWeather);
    }

    if(_isLedDisplayActive){
        switch (_currentLedDisplayMode){
            case LedDisplayMode::OFF: _ledDisplayController->clearScreen();break;
            case LedDisplayMode::TEMPERATURE_OUTSIDE: _ledDisplayController->showOutdoorTemperature(_currentWeather);break;
            case LedDisplayMode::AIR_POLLUTION: _ledDisplayController->showAirPollution(_currentWeather);break;
        }   
    }
    else _ledDisplayController->clearScreen();

    if(!_isMenuMode){        
        if(_isScreenActive){
            switch (_currentScreenMode){
                case ScreenMode::OFF: _screen->clearScreen();break;
                case ScreenMode::TEMPERATURE_OUTSIDE: _screen->showOutdoorTemperature(_currentWeather);break;
                case ScreenMode::AIR_POLLUTION: _screen->showAirPollution(_currentWeather);break;
                case ScreenMode::METEO_OUTSIDE: _screen->showOutdoorHumidityAndPressure(_currentWeather);break;
                case ScreenMode::METEO_INSIDE: _screen->showIndoorWeather(_currentWeather);break;
                case ScreenMode::AIR_QULITY: _screen->showAirQualityMeasurements(_currentWeather);break;
            } 
        }       
        else _screen->clearScreen();
    }
}

void UIController::flipScreenMode(bool forward){
    int currentModeValue = (int)_currentScreenMode;
    if(forward) currentModeValue++;
    else currentModeValue--;

    if(currentModeValue > (int)ScreenMode::AIR_QULITY) currentModeValue = (int)ScreenMode::TEMPERATURE_OUTSIDE;
    if(currentModeValue < (int)ScreenMode::TEMPERATURE_OUTSIDE) currentModeValue = (int)ScreenMode::AIR_QULITY;

    _currentScreenMode = static_cast<ScreenMode>(currentModeValue);
}

void UIController::enableMenuMode(bool debugMode){
    _isMenuMode = true;
    _menuController->showMenu(!debugMode ? MAIN_MENU_MODE : DEBUG_MENU_MODE);
}

void UIController::disableMenuMode(){
    _isMenuMode = false;
    redrawUI();
}

void UIController::onInteraction(){
    _lastInteractionTimestamp = globalSystemState->getCurrentTimestamp();
    updateOutputDevicesStatus();
}

void UIController::updateOutputDevicesStatus(){
    bool oldIsScreenActive = _isScreenActive;
    bool oldIsLedDisplayActive = _isLedDisplayActive;

    _isInteraction = (globalSystemState->getCurrentTimestamp() - _lastInteractionTimestamp) 
                        < USER_INTERACTION_TIMEOUT_SECONDS;

    if(_isInteraction){
        _isScreenActive = true;
        _isLedDisplayActive = true;
    }
    else {
        _isScreenActive = false;
        _isMenuMode = false;
        _isLedDisplayActive = !globalSystemState->isNightMode; 
    }

    //UI redraw required
    if(_isScreenActive != oldIsScreenActive || 
       _isLedDisplayActive != oldIsLedDisplayActive)redrawUI();
}