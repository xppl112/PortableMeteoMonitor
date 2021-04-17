#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/MCPExtender.h"
#include "Models/AllSensorsData.h"
#include <Ticker.h>
#include "McpRGBLed.h"
#include "Log4Esp.h"

class LEDIndicatorsController
{
public:
    LEDIndicatorsController(HardwareRegistry* hardwareRegistry, Logger* logger);
    void setWeatherStatus(AllSensorsData data);
    void clearAllIndicators();

private:
    Logger* _logger;

    McpRGBLed _topLed;
    McpRGBLed _bottomLed;

    Ticker* _timerSlow;
    Ticker* _timerFast;
    bool _fastBlinkingLedOn = false;
    bool _slowBlinkingLedOn = false;
};