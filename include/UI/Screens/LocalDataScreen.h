#pragma once

#include "Models/PresentingWeatherData.h"
#include "UI/Screens/Elements/MetricTile.h"
#include <Adafruit_ST7789.h> 

class LocalDataScreen
{
public:
    LocalDataScreen(Adafruit_ST7789* screen);

    void showWeatherData(PresentingWeatherData weatherData);

private:
    Adafruit_ST7789* _screen;
    MetricTile *_co2Tile, *_PMTile, *_ch2oTile, *_tempTile, *_humTile, *_pressureTile;

    TileStatus mapWarningLevelToTileStatus(WarningLevel level);
};