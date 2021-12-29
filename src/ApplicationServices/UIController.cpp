#include "ApplicationServices/UIController.h"
#include "Config.h"

UIController::UIController(HardwareRegistry* HardwareRegistry, Logger* logger)
{
    _logger = logger;
    _screen = new ScreenController(HardwareRegistry, logger);
    _ledIndicators = new LEDIndicatorsController(HardwareRegistry, logger);
    _inputsController = new InputsController(HardwareRegistry, logger);

    _screen->showSplashScreen();
}

void UIController::updateUI() {
    updateInputs();
}

void UIController::updateInputs() {
    ButtonPressed buttonPressed = _inputsController->updateInputs();

    switch(buttonPressed){
        case ButtonPressed::TOUCH:
            _ledIndicators->setButtonTest(1);
            break;
        case ButtonPressed::LEFT:
            _ledIndicators->setButtonTest(2);
            break;
        case ButtonPressed::CENTER:
            _ledIndicators->setButtonTest(3);
            break;
        case ButtonPressed::RIGHT:
            _ledIndicators->setButtonTest(4);
            break;
        case ButtonPressed::NONE:break;
    }
}

void UIController::onWeatherUpdated(WeatherMonitorData weatherMonitorData){
    _currentWeather = weatherMonitorData;
    redrawUI();
}

void UIController::redrawUI(){
    _ledIndicators->setWeatherStatus(_currentWeather);

    switch (_currentScreenMode){
        case ScreenMode::OFF: _screen->clearScreen();break;
        case ScreenMode::MAIN_DASHBOARD: _screen->showMainScreen(_currentWeather);break;
    } 
}