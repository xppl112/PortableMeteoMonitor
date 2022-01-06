#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <Arduino.h>
#include "Models/Enums/Mode.h"

class GlobalState {    
public:
    GlobalState(){
    }

    volatile Mode mode = Mode::STANDARD;
    volatile bool isNightMode = false;

    unsigned long getCurrentTimestamp(){
        if(_timeStampSnapshot == 0) return 0;
        unsigned long millisDiff = millis() - _millisSnaphot;
        return _timeStampSnapshot + (millisDiff / 1000);
    }

    void updateTime(unsigned long currentTimestamp){
        _timeStampSnapshot = currentTimestamp;
        _millisSnaphot = millis();
    }

private:
    unsigned long _timeStampSnapshot = 0;
    unsigned long _millisSnaphot;

};
#endif