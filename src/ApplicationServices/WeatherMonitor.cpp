#include "ApplicationServices/WeatherMonitor.h"
#include "Config.h"
#include "GlobalObjects/GlobalSystemState.h"
extern GlobalSystemState* globalSystemState;

WeatherMonitor::WeatherMonitor(HardwareRegistry* HardwareRegistry){
    _HardwareRegistry = HardwareRegistry;
    _timer = new Ticker(0);
}

void WeatherMonitor::run(){
    state = WeatherMonitorState::IDLE;
    _timer->start();    
}

void WeatherMonitor::updateTimers(){
    if(globalSystemState->systemHealth != HealthStatus::HEALTH_ERROR)
        _timer->update();

    if(_timer->state() == FIRED){
        if(state == WeatherMonitorState::IDLE)startMeasuring();
        else if(state == WeatherMonitorState::MEASURING)finishMeasuring();
        _timer->resetStatus();
    }
}

void WeatherMonitor::addUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback){
    _onUpdateCallback = callback;
}

void WeatherMonitor::startMeasuring(){
    _timer->interval(WEATHER_MONITOR_MEASUREMENT_DURATION_SECONDS * 1000);
    state = WeatherMonitorState::MEASURING;    
    globalSystemState->setSystemStatus(SystemStatus::Measuring);    
    _startMeasuringTimestamp = globalSystemState->getCurrentTimestamp();
    
    _HardwareRegistry->airParticiplesSensor->beginMeasurement();
}

void WeatherMonitor::finishMeasuring(){
    if(globalSystemState->isNightMode)_timer->interval(WEATHER_MONITOR_INTERVAL_NIGHT_SECONDS * 1000);
    else _timer->interval(WEATHER_MONITOR_INTERVAL_SECONDS * 1000);
    WeatherMonitorData data;

    PmsData airParticiplesData = _HardwareRegistry->airParticiplesSensor->endMeasurement();
    BME280Data outdoorMeteoData = _HardwareRegistry->outdoorMeteoSensor->getData();
    DHTData indoorMeteoData = _HardwareRegistry->indoorMeteoSensor->getData();

    data.isOutsideMeteoDataReceived = outdoorMeteoData.isDataReceived;
    data.isInsideMeteoDataReceived = indoorMeteoData.isDataReceived;
    data.isPMDataReceived = airParticiplesData.isDataReceived;
    data.timeStampOfStart = _startMeasuringTimestamp;
    data.timeStampOfFinish = globalSystemState->getCurrentTimestamp();

    if(airParticiplesData.isDataReceived){
        data.PM1_0 = airParticiplesData.PM_1_0;
        data.PM2_5 = airParticiplesData.PM_2_5;
        data.PM_10_0 = airParticiplesData.PM_10_0;
    }

    if(outdoorMeteoData.isDataReceived){
        data.temperatureOutside = outdoorMeteoData.temperatureCelsium;
        data.humidityOutside = outdoorMeteoData.humidityPercent;
        data.pressureOutside = outdoorMeteoData.pressureInHPascals;
    }

    if(indoorMeteoData.isDataReceived){
        data.temperatureInside = indoorMeteoData.temperatureCelsium;
        data.humidityInside = indoorMeteoData.humidityPercent;
    }

    state = WeatherMonitorState::IDLE;
    globalSystemState->setSystemStatus(SystemStatus::Idle);    
    if(_onUpdateCallback != NULL) _onUpdateCallback(data);
}