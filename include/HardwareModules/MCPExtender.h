#ifndef MCPExtender_H
#define MCPExtender_H

#include "HardwareModules/IHardwareModule.h"
#include "MCP.h"
#include <Arduino.h>

class MCPExtender : public IHardwareModule
{
public:
    MCPExtender(uint8_t i2c_address);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "MCP23017",
            .ConnectionType = HardwareModuleConnectionType::I2C,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 0,
            .IsCritical = true
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    MCP* get(){ return _mcp; }

private:
    MCP *_mcp;
};

#endif