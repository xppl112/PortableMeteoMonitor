#include "ApplicationServices/WeatherMonitor.h"
#include "Config.h"

WeatherMonitor::WeatherMonitor(HardwareRegistry* hardwareRegistry){
    _airParticiplesSensor = hardwareRegistry->_airParticiplesSensor;
    _CH2OSensor = hardwareRegistry->_CH2OSensor;
    _CO2Sensor = hardwareRegistry->_CO2Sensor;
    _meteoSensor = hardwareRegistry->_meteoSensor;
    
    _timer = new Ticker(0);
}

void WeatherMonitor::run(){
    state = WeatherMonitorState::MEASURING;
    _timer->start(true);
}

void WeatherMonitor::stop(){
    _timer->stop();
    _airParticiplesSensor->sleep();
}

void WeatherMonitor::updateTimers(){
    _timer->update();
    if(_timer->state() == FIRED){
        if(state == WeatherMonitorState::IDLE){
            startMeasuring();
            state = WeatherMonitorState::MEASURING;
            _timer->interval(_measurementDurationSeconds * 1000);
        }
        else if(state == WeatherMonitorState::MEASURING){
            finishMeasuring();
            state = WeatherMonitorState::IDLE;   
            _timer->interval(_calmdownDurationSeconds * 1000);
        }
        _timer->start();
    }
}

void WeatherMonitor::startMeasuring(){
    _airParticiplesSensor->wakeUp();
}

void WeatherMonitor::finishMeasuring(){
    WeatherMonitorData data;
    data.timestamp = millis();

    if(_onBlockingCallback != NULL)_onBlockingCallback(true);

    AirParticiplesSensorData airParticiplesData;
    airParticiplesData = _airParticiplesSensor->getData();
    if(_calmdownDurationSeconds != 0)_airParticiplesSensor->sleep();

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

    if(_onBlockingCallback != NULL)_onBlockingCallback(false);
}

void WeatherMonitor::registerWeatherData(WeatherMonitorData data){
    _weatherMonitorHistoricalData.push_back(data);
    if(_weatherMonitorHistoricalData.size() > DATA_COLLECTION_CAPACITY){
        _weatherMonitorHistoricalData.erase(_weatherMonitorHistoricalData.begin());
    }

    if(_onUpdateCallback != NULL){
        PresentingWeatherData PresentingWeatherData {
            .weatherMonitorHistoricalData = _weatherMonitorHistoricalData
        };

        if(data.CO2 >= CO2_LEVEL_ALERT) PresentingWeatherData.CO2WarningLevel = WarningLevel::HI_WARNING_LEVEL;
        else if(data.CO2 >= CO2_LEVEL_WARNING) PresentingWeatherData.CO2WarningLevel = WarningLevel::WARNING;
        else if(data.CO2 != -1) PresentingWeatherData.CO2WarningLevel = WarningLevel::LOW_WARNING_LEVEL;

        if(data.PM_2_5 >= PM2_5_LEVEL_ALERT) PresentingWeatherData.PMWarningLevel = WarningLevel::HI_WARNING_LEVEL;
        else if(data.PM_2_5 >= PM2_5_LEVEL_WARNING) PresentingWeatherData.PMWarningLevel = WarningLevel::WARNING;
        else if(data.PM_2_5 != -1) PresentingWeatherData.PMWarningLevel = WarningLevel::LOW_WARNING_LEVEL;
        
        if(data.CH2O >= CH2O_LEVEL_ALERT) PresentingWeatherData.CH2OWarningLevel = WarningLevel::HI_WARNING_LEVEL;
        else if(data.CH2O >= CH2O_LEVEL_WARNING) PresentingWeatherData.CH2OWarningLevel = WarningLevel::WARNING;
        else if(data.CH2O != -1)PresentingWeatherData.CH2OWarningLevel = WarningLevel::LOW_WARNING_LEVEL;

        _onUpdateCallback(PresentingWeatherData);
    }
}

void WeatherMonitor::setMeasurementMode(Mode mode){
    switch(mode){
        case Mode::STANDARD:
            _measurementDurationSeconds = DEFAULT_MEASUREMENT_DURATION_SECONDS;
            _calmdownDurationSeconds = DEFAULT_CALMDOWN_DURATION_SECONDS;
            break;
        case Mode::ACTIVE_MONITORING:
            _measurementDurationSeconds = ACTIVE_MEASUREMENT_DURATION_SECONDS;
            _calmdownDurationSeconds = ACTIVE_CALMDOWN_DURATION_SECONDS;
            break;
        case Mode::NIGHT:
            _measurementDurationSeconds = NIGHT_MEASUREMENT_DURATION_SECONDS;
            _calmdownDurationSeconds = NIGHT_CALMDOWN_DURATION_SECONDS;
            break;
    };

    state = WeatherMonitorState::IDLE;
    _timer->start(true);
    _weatherMonitorHistoricalData.clear();
    if(_onUpdateCallback != NULL){
        PresentingWeatherData PresentingWeatherData {
            .weatherMonitorHistoricalData = _weatherMonitorHistoricalData
        };
        _onUpdateCallback(PresentingWeatherData);
    }
}