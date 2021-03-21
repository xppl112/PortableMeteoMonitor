#ifndef MCPEXTENDER_H
#define MCPEXTENDER_H

#include <stdint.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define MCP_A0 0
#define MCP_A1 1
#define MCP_A2 2
#define MCP_A3 3
#define MCP_A4 4
#define MCP_A5 5
#define MCP_A6 6
#define MCP_A7 7
#define MCP_B0 8
#define MCP_B1 9
#define MCP_B2 10
#define MCP_B3 11
#define MCP_B4 12
#define MCP_B5 13
#define MCP_B6 14
#define MCP_B7 15

class MCPExtender
{
public:
  MCPExtender(uint8_t i2c_address);

  void connect();
  void pinMode(uint8_t pin, uint8_t direction, int8_t pullup = -1);
  uint8_t digitalRead(uint8_t pin);
  void digitalWrite(uint8_t pin, uint8_t value);

  bool isConnected = false;
private:
  Adafruit_MCP23017* _mcp;
  uint8_t _i2c_address;
};

#endif