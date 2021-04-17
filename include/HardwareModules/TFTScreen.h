#ifndef TFTScreen_H
#define TFTScreen_H

#include "HardwareModules/IHardwareModule.h"
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <Arduino.h>

#define USE_MCP_FOR_TFT
#ifdef USE_MCP_FOR_TFT
#include "MCP.h"
#endif

class TFTScreen : public IHardwareModule
{
public:
    TFTScreen(
        int width, 
        int height, 
        uint8_t csPin, 
        uint8_t dcPin, 
        uint8_t resetPin, 
        uint8_t backlightPin,
        uint8_t spiMode = SPI_MODE2,
        bool useMcpForReset = false,
        bool useMcpForBacklight = false
    );
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "ST7789 IPS",
            .ConnectionType = HardwareModuleConnectionType::SPI,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 100,
            .IsCritical = true
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    void backLight(bool enable = true);

    Adafruit_ST7789* get(){ return _screen; }

    #ifdef USE_MCP_FOR_TFT
    void useMCP(MCP *mcp) {_mcp = mcp;};
    #endif

private:
    Adafruit_ST7789 *_screen;
    #ifdef USE_MCP_FOR_TFT
    MCP *_mcp;
    #endif

    int _width;
    int _height; 
    uint8_t _resetPin;
    uint8_t _backlightPin;
    uint8_t _spiMode;
    bool _useMcpForReset;
    bool _useMcpForBacklight;    
};

#endif
