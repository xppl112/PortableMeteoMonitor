#ifndef DashboardScreen_H
#define DashboardScreen_H

#include "Models/PresentingData.h"
#include "UI/Screens/Elements/MetricTile.h"
#include <Adafruit_ST7789.h> 

class DashboardScreen
{
public:
    DashboardScreen(Adafruit_ST7789* screen);

    void show(PresentingData data);

private:
    Adafruit_ST7789* _screen;
    MetricTile *_co2Tile, *_PMTile, *_ch2oTile, *_outTempTile, *_outPMTile;
};

#endif