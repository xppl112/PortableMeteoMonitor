#pragma once

#include "Models/PresentingBackendWeatherData.h"
#include "UI/Screens/Elements/MetricTile.h"
#include <Adafruit_ST7789.h> 

class OutWeatherScreen
{
public:
    OutWeatherScreen(Adafruit_ST7789* screen);

    void showBackendWeatherData(PresentingBackendWeatherData backendWeatherData);

private:
    Adafruit_ST7789* _screen;
    MetricTile *_outTempTile, *_outHumTile, *_balconyTempTile, *_balconyHumTile, *_outPMTile, *_outPressureTile;

    TileStatus mapWarningLevelToTileStatus(WarningLevel level);
};