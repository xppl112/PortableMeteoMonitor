#ifndef HardwareRegistryFactory_H
#define HardwareRegistryFactory_H

#include <map>
#include "HardwareModules/HardwareRegistry.h"

#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "HardwareModules/Sensors/CH2OSensor.h"
#include "HardwareModules/Sensors/CO2Sensor.h"
#include "HardwareModules/Sensors/MeteoSensor.h"
#include "HardwareModules/Sensors/LightSensor.h"
#include "HardwareModules/MCPExtender.h"
#include "HardwareModules/TFTScreen.h"
#include "HardwareModules/BuzzerModule.h"

class HardwareRegistryFactory
{
public:
    static HardwareRegistry* createHardwareRegistry();

private:
    static HardwareRegistry *_hardwareRegistry;
    /*
    static AirParticiplesSensor *_airParticiplesSensor;
    static CH2OSensor *_CH2OSensor;
    static CO2Sensor *_CO2Sensor;
    static MeteoSensor *_meteoSensor;
    static LightSensor *_lightSensor;
    static MCPExtender *_MCPExtender;
    static TFTScreen *_TFTScreen;
    static BuzzerModule *_buzzerModule;
    */
};

#endif