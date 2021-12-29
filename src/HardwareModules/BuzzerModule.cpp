#include "HardwareModules/BuzzerModule.h"

BuzzerModule::BuzzerModule(MCP *mcp, uint8_t pin)
{
    _mcp = mcp;
    _pin = pin;
}

void BuzzerModule::connect(){
    _mcp->pinMode(_pin, OUTPUT);
    _mcp->digitalWrite(_pin, LOW);
    _isConnected = true;
}

void BuzzerModule::reset(){
}

void BuzzerModule::beep(uint8_t count, short beepLength, short pauseLength) {
    for(uint8_t i = 0; i < count; i++){
        _mcp->digitalWrite(_pin, HIGH);
        delay(beepLength);
        _mcp->digitalWrite(_pin, LOW);
        if(i+1 != count)delay(pauseLength);
    }
}