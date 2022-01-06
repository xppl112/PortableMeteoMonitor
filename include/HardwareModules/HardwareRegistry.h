#ifndef HardwareRegistry_H
#define HardwareRegistry_H

#include "HardwareModules/IHardwareModule.h"
#include <stdint.h>
#include <Ticker.h>
#include "Log4Esp.h"

#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "HardwareModules/Sensors/CH2OSensor.h"
#include "HardwareModules/Sensors/CO2Sensor.h"
#include "HardwareModules/Sensors/MeteoSensor.h"
#include "HardwareModules/Sensors/LightSensor.h"
#include "HardwareModules/MCPExtender.h"
#include "HardwareModules/TFTScreen.h"
#include "HardwareModules/BuzzerModule.h"

class HardwareRegistry
{
public:
    HardwareRegistry(Logger* logger);

    void healthCheck();
    void reconnectAllDisconnectedDevices();
    
    AirParticiplesSensor *_airParticiplesSensor;
    CH2OSensor *_CH2OSensor;
    CO2Sensor *_CO2Sensor;
    MeteoSensor *_meteoSensor;
    LightSensor *_lightSensor;
    MCPExtender *_MCPExtender;
    TFTScreen *_TFTScreen;
    BuzzerModule *_buzzerModule;

private:
    Logger* _logger;
    Ticker* _timer;

    void initializeDevices();
};

#endif