#ifndef BuzzerModule_H
#define BuzzerModule_H

#include "HardwareModules/IHardwareModule.h"
#include <Arduino.h>
#include "MCP.h"

class BuzzerModule : public IHardwareModule
{
public:
    BuzzerModule(MCP *mcp, uint8_t pin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "Active buzzer (MCP)",
            .ConnectionType = HardwareModuleConnectionType::DIGITAL_PINS,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 20,
            .IsCritical = false
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    void beep(uint8_t count = 1, short beepLength = 300, short pauseLength = 300);

private:
    MCP *_mcp;
    uint8_t _pin;
  
};

#endif