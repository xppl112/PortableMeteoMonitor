#ifndef DashboardScreen_H
#define DashboardScreen_H

#include "Models/WeatherMonitorData.h"
#include <Adafruit_ST7789.h> 

class DashboardScreen
{
public:
    DashboardScreen(Adafruit_ST7789* screen);

    void show(WeatherMonitorData data);
    
private:
    Adafruit_ST7789* _screen;
};

#endif