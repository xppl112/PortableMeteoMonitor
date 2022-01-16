#pragma once

#include <vector>
#include "Models/WeatherMonitorData.h"
#include "Models/Enums/WarningLevel.h"

struct PresentingWeatherData
{
public:
    std::vector<WeatherMonitorData> weatherMonitorHistoricalData;

    WarningLevel CO2WarningLevel = WarningLevel::UNKNOWN;
    WarningLevel PMWarningLevel = WarningLevel::UNKNOWN;
    WarningLevel CH2OWarningLevel = WarningLevel::UNKNOWN;

    WarningLevel GetOverallWarningLevel(){
        if(CO2WarningLevel == WarningLevel::HI_WARNING_LEVEL || 
        PMWarningLevel == WarningLevel::HI_WARNING_LEVEL ||
        CH2OWarningLevel == WarningLevel::HI_WARNING_LEVEL)
            return WarningLevel::HI_WARNING_LEVEL;
        else if(CO2WarningLevel == WarningLevel::WARNING || 
        PMWarningLevel == WarningLevel::WARNING ||
        CH2OWarningLevel == WarningLevel::WARNING)
            return WarningLevel::WARNING;
        else if(CO2WarningLevel == WarningLevel::LOW_WARNING_LEVEL || 
        PMWarningLevel == WarningLevel::LOW_WARNING_LEVEL ||
        CH2OWarningLevel == WarningLevel::LOW_WARNING_LEVEL)
            return WarningLevel::LOW_WARNING_LEVEL;
        
        return WarningLevel::UNKNOWN;
    }
};