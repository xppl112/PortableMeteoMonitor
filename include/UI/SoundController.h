#pragma once

#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/BuzzerModule.h"
#include "Models/Enums/WarningLevel.h"

class SoundController
{
public:
    SoundController(HardwareRegistry* hardwareRegistry);
    void setWarningLevel(WarningLevel level);
    void enableAlerting(WarningLevel currentLevel);

    void shortBeep(uint8_t count = 1);
    void longBeep(uint8_t count = 1);

private:
    BuzzerModule* _buzzerModule;

    WarningLevel _lastWarningLevel;
};