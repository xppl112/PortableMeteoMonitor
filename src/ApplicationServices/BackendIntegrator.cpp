#include "ApplicationServices/BackendIntegrator.h"
#include "GlobalObjects/GlobalSystemState.h"
#include "Config.h"
extern GlobalSystemState* globalSystemState;

BackendIntegrator::BackendIntegrator(HealthcheckController* healthcheckController){
    _healthcheckController = healthcheckController;
    BackendClientConfig config;
    _backendClient = new BackendClient(config);

    _statusReportTimer = new Ticker(SYSTEM_STATUS_REPORT_INTERVAL_SECONDS * 1000, NULL, MILLIS);
    _timeUpdateTimer = new Ticker(TIME_UPDATE_INTERVAL_SECONDS * 1000, NULL, MILLIS);
    _statusReportTimer->start();
    _timeUpdateTimer->start(true); 
}
   
void BackendIntegrator::updateTimers(){
    _statusReportTimer->update();
    _timeUpdateTimer->update();

    if(_statusReportTimer->state() == FIRED){
        sendSystemStatusReport();
        _statusReportTimer->resetStatus();
    }
    if(_timeUpdateTimer->state() == FIRED){
        if(!globalSystemState->isNightMode)updateServerTime();
        _timeUpdateTimer->resetStatus();
    }    
}

void BackendIntegrator::updateServerTime(){
    auto currentSystemStatus = globalSystemState->systemStatus;
    globalSystemState->setSystemStatus(SystemStatus::DataTransfer);  

    try {
        int serverTimestamp = _backendClient->GetServerTime();
        if(serverTimestamp != -1) globalSystemState->updateTime(serverTimestamp);
    }
    catch(const std::exception& e){
        globalSystemState->addError(
            SystemErrorCode::HandledNoncriticalException,
            SystemErrorSeverity::SystemWarning,
            "updateServerTime: " + String(e.what())
        );        
    }   
    catch(...){
        globalSystemState->addError(
            SystemErrorCode::HandledNoncriticalException,
            SystemErrorSeverity::SystemWarning,
            "updateServerTime");        
    }   

    globalSystemState->setSystemStatus(currentSystemStatus);     
}

void BackendIntegrator::onWeatherUpdated(WeatherMonitorData weatherMonitorData){
    collectWeatherUpdate(weatherMonitorData);

    if(!globalSystemState->isNightMode){
        auto currentSystemStatus = globalSystemState->systemStatus;
        globalSystemState->setSystemStatus(SystemStatus::DataTransfer);  

        try {
            _backendClient->SendWeatherData(&_weatherMonitorDataCollection);
        }
        catch(const std::exception& e){
            globalSystemState->addError(
                SystemErrorCode::HandledNoncriticalException,
                SystemErrorSeverity::SystemWarning,
                "onWeatherUpdated: " + String(e.what())
            );        
        }           
        catch(...){
            globalSystemState->addError(
                SystemErrorCode::HandledNoncriticalException,
                SystemErrorSeverity::SystemWarning,
                "onWeatherUpdated");        
        }    

        globalSystemState->setSystemStatus(currentSystemStatus);    
    }
 
    globalSystemState->unsyncronizedWeatherReports = _weatherMonitorDataCollection.size();
}

void BackendIntegrator::sendSystemStatusReport(){
    collectSystemReport(_healthcheckController->getSystemHealthReport());

    if(!globalSystemState->isNightMode){
        auto currentSystemStatus = globalSystemState->systemStatus;
        globalSystemState->setSystemStatus(SystemStatus::DataTransfer);  

        try {
            _backendClient->SendSystemStatusReports(&_systemHealthReportsCollection);
        }
        catch(const std::exception& e){
            globalSystemState->addError(
                SystemErrorCode::HandledNoncriticalException,
                SystemErrorSeverity::SystemWarning,
                "sendSystemStatusReport: " + String(e.what())
            );        
        } 
        catch(...){
            globalSystemState->addError(
                SystemErrorCode::HandledNoncriticalException,
                SystemErrorSeverity::SystemWarning,
                "sendSystemStatusReport");        
        }   

        globalSystemState->setSystemStatus(currentSystemStatus);
    }
    
    globalSystemState->unsyncronizedHealthReports = _systemHealthReportsCollection.size();
}

void BackendIntegrator::collectWeatherUpdate(WeatherMonitorData weatherMonitorData){
    _weatherMonitorDataCollection.push(weatherMonitorData);
    if(_weatherMonitorDataCollection.size() > DATA_COLLECTION_CAPACITY)_weatherMonitorDataCollection.pop();
}

void BackendIntegrator::collectSystemReport(SystemHealthReport systemHealthReport){
    _systemHealthReportsCollection.push(systemHealthReport);
    if(_systemHealthReportsCollection.size() > DATA_COLLECTION_CAPACITY)_systemHealthReportsCollection.pop();
}