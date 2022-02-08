#pragma once

#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/MCPExtender.h"
#include "Models/PresentingWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include <Ticker.h>
#include "McpRGBLed.h"

class LEDIndicatorsController
{
public:
    LEDIndicatorsController(HardwareRegistry* hardwareRegistry);
    void setWeatherStatus(PresentingWeatherData data);
    void setWeatherStatus(PresentingBackendWeatherData data);
    void clearAllIndicators();

private:
    McpRGBLed* _topLed;
    McpRGBLed* _bottomLed;

    Ticker* _timerSlow;
    Ticker* _timerFast;
    bool _fastBlinkingLedOn = false;
    bool _slowBlinkingLedOn = false;
};