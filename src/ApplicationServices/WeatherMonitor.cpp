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
    data.timestamp = millis();

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

    registerWeatherData(data);
}

void WeatherMonitor::registerWeatherData(WeatherMonitorData data){
    _weatherMonitorHistoricalData.push_back(data);
    if(_weatherMonitorHistoricalData.size() > DATA_COLLECTION_CAPACITY){
        _weatherMonitorHistoricalData.erase(_weatherMonitorHistoricalData.begin());
    }

    if(_onUpdateCallback != NULL){
        PresentingData presentingData {
            .weatherMonitorHistoricalData = _weatherMonitorHistoricalData
        };

        if(data.CO2 > CO2_LEVEL_WARNING) presentingData.CO2WarningLevel = WarningLevel::WARNING;
        else if(data.CO2 > CO2_LEVEL_ALERT) presentingData.CO2WarningLevel = WarningLevel::HI_WARNING_LEVEL;

        if(data.PM_2_5 > PM2_5_LEVEL_WARNING) presentingData.PMWarningLevel = WarningLevel::WARNING;
        else if(data.PM_2_5 > PM2_5_LEVEL_ALERT) presentingData.PMWarningLevel = WarningLevel::HI_WARNING_LEVEL;

        if(data.CH2O > CH2O_LEVEL_WARNING) presentingData.CH2OWarningLevel = WarningLevel::WARNING;
        else if(data.CH2O > CH2O_LEVEL_ALERT) presentingData.CH2OWarningLevel = WarningLevel::HI_WARNING_LEVEL;

         _onUpdateCallback(presentingData);
    }
}