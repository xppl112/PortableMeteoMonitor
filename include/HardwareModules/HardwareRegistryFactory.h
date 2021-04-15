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

class HardwareRegistryFactory
{
public:
    static HardwareRegistry createHardwareRegistry();
};

#endif