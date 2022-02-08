#include "ApplicationServices/BackendIntegrator.h"
#include <ArduinoJson.h>

BackendIntegrator::BackendIntegrator(){
    _esp = new EspWifiClient();
    _timer = new Ticker(_backendRefreshIntervalSeconds * 1000);
}

void BackendIntegrator::run(){
    _timer->start(true);
}

void BackendIntegrator::stop(){
    _timer->stop();
    disconnectWifi();
}

void BackendIntegrator::updateTimers(){
    _timer->update();
    if(_timer->state() == FIRED){
        requestWeatherData();
        _timer->start();
    }
}

void BackendIntegrator::requestWeatherData(){
    if(_onBlockingCallback != NULL)_onBlockingCallback(true);
    if(_onNetworkStatusChangeCallback != NULL)_onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);
    
    if(!connectWifi()){
        if(_onNetworkStatusChangeCallback != NULL)_onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
        if(_onBlockingCallback != NULL)_onBlockingCallback(false);
        return;
    }
    
    HttpResponse response = _esp->sendGetRequest(
        SERVER_HOST, String(SERVER_API_GET_OUTDOOR_WEATHER) + "?c=" + String(DATA_COLLECTION_CAPACITY));

    if(response.success && response.statusCode == 200){
        if(_onNetworkStatusChangeCallback != NULL)_onNetworkStatusChangeCallback(NetworkStatus::STANDBY);

        DynamicJsonDocument jsonDoc(4000);
        deserializeJson(jsonDoc, response.payload);
        //unsigned long currentTimestamp = jsonDoc["cts"];
        JsonArray dataArray = jsonDoc["data"].as<JsonArray>();

        _backendWeatherHistoricalData.clear();
        BackendWeatherData wd;
        for (JsonObject dataItem : dataArray) {
            long ts = ((millis()/1000) - (long)dataItem["ts"]) * 1000;
            wd.timestamp = ts;
            wd.PM_2_5 = dataItem["PM"];
            wd.temperatureCelsium = dataItem["to"];
            wd.humidityPercent = dataItem["ho"];
            wd.pressureInHPascals = 1000.0+(float)dataItem["p"];
            wd.temperatureBalconyCelsium = dataItem["tb"];
            wd.humidityBalconyPercent = dataItem["hb"];

            _backendWeatherHistoricalData.push_back(wd);
        }

        if(_onUpdateCallback != NULL){
            PresentingBackendWeatherData presentingBackendWeatherData {
                .backendWeatherHistoricalData = _backendWeatherHistoricalData
            };

            if(wd.PM_2_5 >= PM2_5_LEVEL_ALERT) presentingBackendWeatherData.PMWarningLevel = WarningLevel::HI_WARNING_LEVEL;
            else if(wd.PM_2_5 >= PM2_5_LEVEL_WARNING) presentingBackendWeatherData.PMWarningLevel = WarningLevel::WARNING;
            else if(wd.PM_2_5 != -1)presentingBackendWeatherData.PMWarningLevel = WarningLevel::LOW_WARNING_LEVEL;

            _onUpdateCallback(presentingBackendWeatherData);
        }
    }
    else _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
    
    if(_onBlockingCallback != NULL)_onBlockingCallback(false);
}

bool BackendIntegrator::connectWifi(){
    if(!IsConnected || !_esp->isWifiConnected()){
        IsConnected = _esp->connectWifi(WIFI_SSID, WIFI_PASSWORD);
    }

    return IsConnected;
}
void BackendIntegrator::disconnectWifi(){
    _esp->disconnectWifi();
    _onNetworkStatusChangeCallback(NetworkStatus::DISABLED);
}

void BackendIntegrator::setRefreshMode(Mode mode){
    switch(mode){
        case Mode::STANDARD:
        case Mode::ACTIVE_MONITORING:
            _backendRefreshIntervalSeconds = DEFAULT_BACKEND_REFRESH_INTERVAL_SECONDS;
            break;
        case Mode::NIGHT:
            _backendRefreshIntervalSeconds = NIGHT_BACKEND_REFRESH_INTERVAL_SECONDS;
            break;
    };
    _timer->interval(_backendRefreshIntervalSeconds * 1000);
    if(IsConnected)_timer->start();
}