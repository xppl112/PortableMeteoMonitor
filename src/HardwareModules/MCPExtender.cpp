#include "HardwareModules/MCPExtender.h"

MCPExtender::MCPExtender(uint8_t i2c_address)
{
    _mcp = new MCP(i2c_address);
}

void MCPExtender::connect(){
    _mcp->connect();
    _isConnected = _mcp->isConnected;
}

void MCPExtender::reset(){
    connect();
}