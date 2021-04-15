#include "HardwareModules/TFTScreen.h"

TFTScreen::TFTScreen(
    uint8_t width, 
    uint8_t height, 
    uint8_t csPin, 
    uint8_t dcPin, 
    uint8_t resetPin, 
    uint8_t backlightPin,
    uint8_t spiMode,
    bool useMcpForReset,
    bool useMcpForBacklight)
{
    _width = width;
    _height = height;
    _resetPin = resetPin;
    _backlightPin = backlightPin;
    _spiMode = spiMode;
    _useMcpForReset = useMcpForReset;
    _useMcpForBacklight = useMcpForBacklight;

    // if useMcpForReset is true, RST pin is controlled outside the library
    _screen = new Adafruit_ST7789(csPin, dcPin, useMcpForReset ? -1 : resetPin);
}

void TFTScreen::connect(){
    if(_useMcpForReset){
        _screen->initSPI(0, _spiMode);

        // Toggle _rst low to reset
        #ifdef USE_MCP_FOR_TFT
        _mcp->pinMode(_resetPin, OUTPUT);
        _mcp->digitalWrite(_resetPin, HIGH);
        delay(100);
        _mcp->digitalWrite(_resetPin, LOW);
        delay(100);
        _mcp->digitalWrite(_resetPin, HIGH);
        delay(200);
        #endif
    }

    if(!_useMcpForBacklight){
        pinMode(_backlightPin, OUTPUT);
    }
    else {
        #ifdef USE_MCP_FOR_TFT
        _mcp->pinMode(_backlightPin, OUTPUT);
        #endif
    }

    _screen->init(_width, _height, _spiMode);
    _isConnected = true;
}

void TFTScreen::reset(){
    connect();
}

bool TFTScreen::backLight(bool enable){
    if(!_useMcpForBacklight){
        digitalWrite(_backlightPin, enable);
    }
    else {
        #ifdef USE_MCP_FOR_TFT
        _mcp->digitalWrite(_backlightPin, enable);
        #endif
    }
}