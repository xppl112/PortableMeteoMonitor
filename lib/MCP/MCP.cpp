#include <Arduino.h>
#include "MCP.h"

MCP::MCP(uint8_t i2c_address)
{
    _mcp = new Adafruit_MCP23017();
    _i2c_address = i2c_address;
}

void MCP::connect(){
    _mcp->begin((uint8_t)(_i2c_address - 0x20));
    isConnected = true;
}

/*!
 * @brief Sets specified pin mode
 * @param pin Pin to read
 * @param direction INPUT or OUTPUT
 * @param pullup LOW, HIGH or -1
 */
void MCP::pinMode(uint8_t pin, uint8_t direction, int8_t pullup)
{
    _mcp->pinMode(pin, direction);
    if(pullup != -1)_mcp->pullUp(pin, pullup);
}

uint8_t MCP::digitalRead(uint8_t pin)
{
    return _mcp->digitalRead(pin);
}

void MCP::digitalWrite(uint8_t pin, uint8_t value)
{
    _mcp->digitalWrite(pin, value);
}