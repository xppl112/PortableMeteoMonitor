#include "ApplicationServices/SystemModeController.h"
#include "DateTimeUtils.h"
#include "Config.h"
#include "GlobalObjects/GlobalSystemState.h"
extern GlobalSystemState* globalSystemState;

SystemModeController::SystemModeController(){   
    _nightModeTimer = new Ticker(60 * 1000, NULL, MILLIS);
    _nightModeTimer->start();
}

void SystemModeController::refresh() {   
    _nightModeTimer->update();

    if(_nightModeTimer->state() == FIRED){
        checkNightMode();
        _nightModeTimer->resetStatus();
    }    
}

void SystemModeController::checkNightMode() {    
    auto currentTimestamp = globalSystemState->getCurrentTimestamp();
    if(currentTimestamp == 0) return;

    auto curentHour = DateTimeUtils::getHourFromTimestamp(currentTimestamp, TIMEZONE_HOURS_OFFSET);
    if(curentHour >= NIGHT_MODE_HOUR_FROM && curentHour < NIGHT_MODE_HOUR_TO){
        globalSystemState->setNightMode(true);
    }
    else {
        globalSystemState->setNightMode(false);
    }
}