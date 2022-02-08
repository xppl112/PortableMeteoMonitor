#include <Arduino.h>
#include "UI/SoundController.h"
#include "Config.h"

SoundController::SoundController(HardwareRegistry* hardwareRegistry){
    _buzzerModule = hardwareRegistry->_buzzerModule;
    _buzzerModule->connect();
}

void SoundController::setWarningLevel(WarningLevel level){
    if(_lastWarningLevel != level){
        if(_lastWarningLevel != WarningLevel::HI_WARNING_LEVEL && level == WarningLevel::WARNING)shortBeep();
        if(level == WarningLevel::HI_WARNING_LEVEL)shortBeep(2);
    }

    _lastWarningLevel = level;
}

void SoundController::enableAlerting(WarningLevel currentLevel){
    _lastWarningLevel = currentLevel;
    shortBeep();
}

void SoundController::shortBeep(uint8_t count){
    _buzzerModule->beep(count, 200, 200);
}

void SoundController::longBeep(uint8_t count){
    _buzzerModule->beep(count, 400);
}