#include <Arduino.h>
#include "UI/LEDIndicatorsController.h"
#include "Config.h"

LEDIndicatorsController::LEDIndicatorsController(HardwareRegistry* hardwareRegistry, Logger* logger){
    _logger = logger;
    auto mcpExtender = hardwareRegistry->_MCPExtender;

    _topLed = new McpRGBLed(mcpExtender->get(), LED_TOP_RGB_R, LED_TOP_RGB_G, LED_TOP_RGB_B, false);
    _bottomLed = new McpRGBLed(mcpExtender->get(), LED_BOTTOM_RGB_R, LED_BOTTOM_RGB_G, LED_BOTTOM_RGB_B, false);

    _timerSlow = new Ticker(1000, NULL, MILLIS); 
    _timerSlow->start();
    _timerFast = new Ticker(400, NULL, MILLIS); 
    _timerFast->start();
}

void LEDIndicatorsController::setWeatherStatus(PresentingWeatherData data){  
    switch (data.GetOverallWarningLevel())
    {
    case WarningLevel::HI_WARNING_LEVEL:
        _topLed->setColor(RGBLedColor::RED);break;
    case WarningLevel::WARNING:
        _topLed->setColor(RGBLedColor::YELLOW);break;
    case WarningLevel::LOW_WARNING_LEVEL:
        _topLed->setColor(RGBLedColor::GREEN);break;
    default:
        _topLed->setColor(RGBLedColor::BLACK);
    }
}

void LEDIndicatorsController::setWeatherStatus(PresentingBackendWeatherData data){  
    switch (data.GetOverallWarningLevel())
    {
    case WarningLevel::HI_WARNING_LEVEL:
        _bottomLed->setColor(RGBLedColor::RED);break;
    case WarningLevel::WARNING:
        _bottomLed->setColor(RGBLedColor::YELLOW);break;
    case WarningLevel::LOW_WARNING_LEVEL:
        _bottomLed->setColor(RGBLedColor::BLACK);break;
    default:
        _bottomLed->setColor(RGBLedColor::BLACK);
    }
}

void LEDIndicatorsController::clearAllIndicators(){
    _topLed->setColor(RGBLedColor::BLACK);
    _bottomLed->setColor(RGBLedColor::BLACK);
}