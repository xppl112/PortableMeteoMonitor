#pragma once

#include <list>
#include "Models/WeatherMonitorData.h"

struct PresentingData
{
public:
    std::list<WeatherMonitorData> weatherMonitorHistoricalData;

};