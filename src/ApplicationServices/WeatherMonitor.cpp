#include "ApplicationServices/WeatherMonitor.h"
#include "Config.h"

WeatherMonitor::WeatherMonitor(HardwareRegistry* hardwareRegistry, Logger* logger){
    _logger = logger;
    
    _airParticiplesSensor = hardwareRegistry->_airParticiplesSensor;
    _CH2OSensor = hardwareRegistry->_CH2OSensor;
    _CO2Sensor = hardwareRegistry->_CO2Sensor;
    _meteoSensor = hardwareRegistry->_meteoSensor;
    
    _timer = new Ticker(0);
}

void WeatherMonitor::run(){
    state = WeatherMonitorState::MEASURING;
    _timer->start(true);
    finishMeasuring(true);
}

void WeatherMonitor::updateTimers(){
    _timer->update();
    if(_timer->state() == FIRED){
        if(state == WeatherMonitorState::IDLE){
            startMeasuring();
            state = WeatherMonitorState::MEASURING;
            _timer->interval(DEFAULT_MEASUREMENT_DURATION_SECONDS * 1000);
        }
        else if(state == WeatherMonitorState::MEASURING){
            finishMeasuring();
            state = WeatherMonitorState::IDLE;   
            _timer->interval(DEFAULT_CALMDOWN_DURATION_SECONDS * 1000);
        }
        _timer->start();
    }
}

void WeatherMonitor::addUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback){
    _onUpdateCallback = callback;
}

void WeatherMonitor::startMeasuring(){
    _airParticiplesSensor->beginMeasurement();
}

void WeatherMonitor::finishMeasuring(bool runWithoutStart){
    WeatherMonitorData data;

    AirParticiplesSensorData airParticiplesData;
    if(!runWithoutStart)airParticiplesData = _airParticiplesSensor->endMeasurement();

    auto ch2oData = _CH2OSensor->getData();
    auto co2Data = _CO2Sensor->getData();
    auto meteoData = _meteoSensor->getData();

    if(airParticiplesData.isDataReceived){
        data.PM_1_0 = airParticiplesData.PM_1_0;
        data.PM_2_5 = airParticiplesData.PM_2_5;
        data.PM_10_0 = airParticiplesData.PM_10_0;
    }

    if(ch2oData.isDataReceived){
        data.CH2O = ch2oData.concentration;
    }

    if(co2Data.isDataReceived){
        data.CO2 = co2Data.concentration;
    }

    if(meteoData.isDataReceived){
        data.temperatureCelsium = meteoData.temperatureCelsium;
        data.humidityPercent = meteoData.humidityPercent;
        data.pressureInHPascals = meteoData.pressureInHPascals;
    }

    if(_onUpdateCallback != NULL) _onUpdateCallback(data);
}