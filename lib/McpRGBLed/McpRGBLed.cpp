#include <Arduino.h>
#include "McpRGBLed.h"

McpRGBLed::McpRGBLed(MCP* mcp, uint8_t pinR, uint8_t pinG, uint8_t pinB, bool commonAnode){
    _mcp = mcp;
    _pinR = pinR;
    _pinG = pinG;
    _pinB = pinB;
    _commonAnode = commonAnode;

    _mcp->pinMode(_pinR, OUTPUT);
    _mcp->pinMode(_pinG, OUTPUT);
    _mcp->pinMode(_pinB, OUTPUT);
}

void McpRGBLed::setColor(RGBLedColor color){
    bool R, G, B = false; 

    switch (color)
    {
        case RGBLedColor::BLACK: R = false; G = false; B = false; break;
        case RGBLedColor::RED: R = true; G = false; B = false; break;
        case RGBLedColor::GREEN: R = false; G = true; B = false; break;
        case RGBLedColor::BLUE: R = false; G = false; B = true; break;
        case RGBLedColor::YELLOW: R = true; G = true; B = false; break;
        case RGBLedColor::CYAN: R = false; G = true; B = true; break;
        case RGBLedColor::PURPLE: R = true; G = false; B = true; break;
        case RGBLedColor::WHITE: R = true; G = true; B = true; break;
        default: break;
    }

    _mcp->digitalWrite(_pinR, _commonAnode ? !R : R);
    _mcp->digitalWrite(_pinG, _commonAnode ? !G : G);
    _mcp->digitalWrite(_pinB, _commonAnode ? !B : B);
}