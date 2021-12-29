#include "ApplicationServices/WeatherMonitor.h"
#include "Config.h"

WeatherMonitor::WeatherMonitor(HardwareRegistry* hardwareRegistry, Logger* logger){
    _logger = logger;
    
    _airParticiplesSensor = hardwareRegistry->getDevice<AirParticiplesSensor>
        ((HardwareId)HardwareIdList::AIR_PARTICIPLES_SENSOR);
    _CH2OSensor = hardwareRegistry->getDevice<CH2OSensor>
        ((HardwareId)HardwareIdList::CH2O_SENSOR);
    _CO2Sensor = hardwareRegistry->getDevice<CO2Sensor>
        ((HardwareId)HardwareIdList::CO2_SENSOR);
    _meteoSensor = hardwareRegistry->getDevice<MeteoSensor>
        ((HardwareId)HardwareIdList::METEO_SENSOR);
    
    _timer = new Ticker(0);
}

void WeatherMonitor::run(){
    state = WeatherMonitorState::IDLE;
    _timer->start();    
}

void WeatherMonitor::updateTimers(){
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
    _timer->interval(PMS_DEFAULT_MEASUREMENT_DURATION_SECONDS * 1000);
    state = WeatherMonitorState::MEASURING;
    
    _airParticiplesSensor->beginMeasurement();
}

void WeatherMonitor::finishMeasuring(){
    _timer->interval(10 * 1000);
    WeatherMonitorData data;

    auto airParticiplesData = _airParticiplesSensor->endMeasurement();
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

    state = WeatherMonitorState::IDLE;   
    if(_onUpdateCallback != NULL) _onUpdateCallback(data);
}