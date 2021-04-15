#include <Arduino.h>
#include "Config.h"
#include "HardwareModules/HardwareRegistry.h"

HardwareRegistry::HardwareRegistry(){    
    _hardwareModules = std::map<HardwareId, IHardwareModule*>();
}

template<class DeviceType>
bool HardwareRegistry::registerDevice(HardwareId deviceId, DeviceType* device){
    if(_hardwareModules[deviceId] != NULL) return false;
    _hardwareModules[deviceId] = device;
    return true;
}

template<class DeviceType>
DeviceType* HardwareRegistry::getDevice(HardwareId deviceId){
    return dynamic_cast<DeviceType*>(_hardwareModules[deviceId]);
}

void HardwareRegistry::reconnectAllDisconnectedDevices(bool onlyCritical){    
    for (auto module : _hardwareModules) {
        if(((onlyCritical && module.second->getInfo().IsCritical) || !onlyCritical) &&
            !module.second->isConnected())module.second->connect();
    }
}

void HardwareRegistry::resetDevice(HardwareId deviceId){
    _hardwareModules[deviceId]->reset();
}
