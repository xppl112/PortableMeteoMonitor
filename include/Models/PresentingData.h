#pragma once

#include <vector>
#include "Models/WeatherMonitorData.h"

enum class WarningLevel { LOW_WARNING_LEVEL, WARNING, HI_WARNING_LEVEL };

struct PresentingData
{
public:
    std::vector<WeatherMonitorData> weatherMonitorHistoricalData;

    WarningLevel CO2WarningLevel = WarningLevel::LOW_WARNING_LEVEL;
    WarningLevel PMWarningLevel = WarningLevel::LOW_WARNING_LEVEL;
    WarningLevel CH2OWarningLevel = WarningLevel::LOW_WARNING_LEVEL;
};