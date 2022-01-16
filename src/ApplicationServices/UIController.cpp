#include "ApplicationServices/UIController.h"
#include "Config.h"

UIController::UIController(HardwareRegistry* HardwareRegistry, Logger* logger)
{
    _logger = logger;
    _screen = new ScreenController(HardwareRegistry, logger);
    _ledIndicators = new LEDIndicatorsController(HardwareRegistry, logger);
    _soundController = new SoundController(HardwareRegistry, logger);
    _inputsController = new InputsController(HardwareRegistry, logger);

    _timer = new Ticker(UI_REDRAW_INTERVAL_MS);
    _timer->start(true);

    //Draw splash data
    onPresentingWeatherDataUpdate(_currentPresentingWeatherData);
    onPresentingBackendWeatherDataUpdate(_currentPresentingBackendWeatherData);
    onNetworkStatusChange(NetworkStatus::DISABLED);
}

void UIController::updateUI() {
    updateInputs();
    
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
            break;
        case ButtonPressed::LEFT:
            break;
        case ButtonPressed::CENTER:
            toggleLedEnabling();
            break;
        case ButtonPressed::RIGHT:
            toggleSoundEnabling();
            break;
        case ButtonPressed::NONE:break;
    }
}

void UIController::onPresentingWeatherDataUpdate(PresentingWeatherData presentingWeatherData){
    _currentPresentingWeatherData = presentingWeatherData;
    
    if(_isLedEnabled)_ledIndicators->setWeatherStatus(_currentPresentingWeatherData);
    if(_isSoundEnabled)_soundController->setWarningLevel(_currentPresentingWeatherData.GetOverallWarningLevel());
    _screen->showDataScreen(_currentView, _currentPresentingWeatherData);
    
    drawInterface();
}

void UIController::onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData presentingBackendWeatherData){
    _currentPresentingBackendWeatherData = presentingBackendWeatherData;
    
    if(_isLedEnabled)_ledIndicators->setWeatherStatus(_currentPresentingBackendWeatherData);
    _screen->showDataScreen(_currentView, _currentPresentingBackendWeatherData);
    
    drawInterface();
}

void UIController::onNetworkStatusChange(NetworkStatus networkStatus){
    _screen->showNetworkStatusIcon(networkStatus);
}

void UIController::onBlocking(bool isBlocked){
    _isBlocked = isBlocked;
    drawInterface();
}

void UIController::drawInterface(){
    _screen->showMainButtons(_isBlocked, _isLedEnabled, _isSoundEnabled);
}

void UIController::toggleLedEnabling(){
    _isLedEnabled = !_isLedEnabled;
    drawInterface();
    if(_isLedEnabled){
        _ledIndicators->setWeatherStatus(_currentPresentingWeatherData);
        _ledIndicators->setWeatherStatus(_currentPresentingBackendWeatherData);
    }
    else _ledIndicators->clearAllIndicators();
}

void UIController::toggleSoundEnabling(){
    _isSoundEnabled = !_isSoundEnabled;
    drawInterface();
    if(_isSoundEnabled){
        _soundController->shortBeep();
    }
}