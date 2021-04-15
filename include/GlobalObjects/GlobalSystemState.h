#ifndef GLOBALSYSTEMSTATE_H
#define GLOBALSYSTEMSTATE_H

#include <unordered_set>
#include <Arduino.h>
#include "Models/SystemErrorCodes.h"
#include "Healthchecks/IHealthcheck.h"

typedef void (*SystemStateUpdatedEventCallback)();

enum class PowerStatus {Unknown, Regular, Reserve, Warning, DangerousVoltage};
enum class SystemStatus {Unknown, Idle, Measuring, DataTransfer};

struct SystemError {
    SystemErrorSeverity severity;
    SystemErrorCode errorCode;
    String description;

    bool operator==(const SystemError &other) const
    { 
        return (errorCode == other.errorCode); 
    }
};

struct SystemErrorHasher
{
   size_t operator () (const SystemError& x) const
   {
      return (unsigned int)x.errorCode;
   }
};


class GlobalSystemState {    
public:
    GlobalSystemState(){
    }

    volatile PowerStatus powerStatus = PowerStatus::Regular;
    volatile SystemStatus systemStatus = SystemStatus::Idle;
    volatile HealthStatus systemHealth = HealthStatus::HEALTH_OK;
    
    volatile bool isNightMode = false;
    volatile uint8_t unsyncronizedWeatherReports = 0;
    volatile uint8_t unsyncronizedHealthReports = 0;

    SystemStateUpdatedEventCallback _onUpdateCallback;

    void addUpdatedEventHandler(SystemStateUpdatedEventCallback callback){
        _onUpdateCallback = callback;
    }

    unsigned long getCurrentTimestamp(){
        if(_timeStampSnapshot == 0) return 0;
        unsigned long millisDiff = millis() - _millisSnaphot;
        return _timeStampSnapshot + (millisDiff / 1000);
    }

    void updateTime(unsigned long currentTimestamp){
        _timeStampSnapshot = currentTimestamp;
        _millisSnaphot = millis();
    }

    void setSystemStatus(SystemStatus status){
        if(getErrorsCount() == 0){
            systemStatus = status;
            _onUpdateCallback();
        }
    }

    void setPowerStatus(PowerStatus status){
        powerStatus = status;
        _onUpdateCallback();
    }

    void setNightMode(bool nightMode){
        isNightMode = nightMode; 
        _onUpdateCallback();
    }

    void addError(SystemErrorCode code, SystemErrorSeverity severity = SystemErrorSeverity::SystemError, String description = ""){
        SystemError error {
            .severity = severity,
            .errorCode = code,
            .description = description
        };        
        _errors.insert(error);
    }

    int getErrorsCount(){ return _errors.size(); }
    std::vector<SystemError> getAllErrors() { 
        std::vector<SystemError> errorsList;
        for(auto error : _errors){
            errorsList.push_back(error);
        }
        return errorsList; 
    }
    void removeError(SystemErrorCode code){
        for(auto error : _errors){
            if(error.errorCode == code){
                _errors.erase(error);
                break;
            }
        }        
    }
    void clearAllErrors(){ _errors.clear(); }

private:
    unsigned long _timeStampSnapshot = 0;
    unsigned long _millisSnaphot;

    std::unordered_set<SystemError, SystemErrorHasher> _errors;
};
#endif