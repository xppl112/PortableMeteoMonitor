#ifndef HardwareRegistry_H
#define HardwareRegistry_H

#include <map>
#include "HardwareModules/IHardwareModule.h"
#include <stdint.h>

typedef uint8_t HardwareId;

class HardwareRegistry
{
public:
    HardwareRegistry();

    template<typename DeviceType>
    bool registerDevice(HardwareId deviceId, DeviceType* device);

    template<typename DeviceType>
    DeviceType* getDevice(HardwareId deviceId);

    void resetDevice(HardwareId deviceId);
    void reconnectAllDisconnectedDevices(bool onlyCritical = false);
    
private:
    std::map<HardwareId, IHardwareModule*> _hardwareModules;
};

#endif