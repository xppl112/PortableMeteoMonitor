#include "ApplicationServices/UIController.h"
#include "Config.h"

UIController::UIController(HardwareRegistry* HardwareRegistry, Logger* logger)
{
    _logger = logger;
    _screen = new ScreenController(HardwareRegistry, logger);
    _ledIndicators = new LEDIndicatorsController(HardwareRegistry, logger);
    _inputsController = new InputsController(HardwareRegistry, logger);

    _timer = new Ticker(UI_REDRAW_INTERVAL_MS);
    _timer->start(true);    
}

void UIController::updateUI() {
    updateInputs();
    
    _timer->update();
    if(_timer->state() == FIRED){
        redrawUI();
        _timer->start();
    }
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

void UIController::onPresentingDataUpdate(PresentingData presentingData){
    _currentPresentingData = presentingData;
    redrawUI();
}

void UIController::redrawUI(){
    _ledIndicators->setWeatherStatus(_currentPresentingData.weatherMonitorHistoricalData.back());
    _screen->showDataScreen(_currentView, _currentPresentingData);
}