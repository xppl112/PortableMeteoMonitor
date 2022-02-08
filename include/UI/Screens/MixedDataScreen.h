#pragma once

#include "Models/PresentingWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include "UI/Screens/Elements/MetricTile.h"
#include <Adafruit_ST7789.h> 

class MixedDataScreen
{
public:
    MixedDataScreen(Adafruit_ST7789* screen);

    void showWeatherData(PresentingWeatherData weatherData);
    void showBackendWeatherData(PresentingBackendWeatherData backendWeatherData);

private:
    Adafruit_ST7789* _screen;
    MetricTile *_co2Tile, *_PMTile, *_ch2oTile, *_outTempTile, *_outPMTile, *_outPressureTile;

    TileStatus mapWarningLevelToTileStatus(WarningLevel level);
};
