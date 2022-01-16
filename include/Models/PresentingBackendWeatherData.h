#pragma once

#include <vector>
#include "Models/BackendWeatherData.h"
#include "Models/Enums/WarningLevel.h"

struct PresentingBackendWeatherData
{
public:
    std::vector<BackendWeatherData> backendWeatherHistoricalData;

    WarningLevel PMWarningLevel = WarningLevel::UNKNOWN;

    WarningLevel GetOverallWarningLevel(){
        if(PMWarningLevel == WarningLevel::HI_WARNING_LEVEL)
            return WarningLevel::HI_WARNING_LEVEL;
        else if(PMWarningLevel == WarningLevel::WARNING)
            return WarningLevel::WARNING;
        else if(PMWarningLevel == WarningLevel::LOW_WARNING_LEVEL)
            return WarningLevel::LOW_WARNING_LEVEL;
        
        return WarningLevel::UNKNOWN;
    }
};