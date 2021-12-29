#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/MCPExtender.h"
#include "Models/WeatherMonitorData.h"
#include <Ticker.h>
#include "McpRGBLed.h"
#include "Log4Esp.h"

class LEDIndicatorsController
{
public:
    LEDIndicatorsController(HardwareRegistry* hardwareRegistry, Logger* logger);
    void setWeatherStatus(WeatherMonitorData data);
    void clearAllIndicators();
    
    void setButtonTest(short button);

private:
    Logger* _logger;

    McpRGBLed* _topLed;
    McpRGBLed* _bottomLed;

    Ticker* _timerSlow;
    Ticker* _timerFast;
    bool _fastBlinkingLedOn = false;
    bool _slowBlinkingLedOn = false;
};