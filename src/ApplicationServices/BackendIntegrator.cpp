#include "ApplicationServices/BackendIntegrator.h"
#include <ArduinoJson.h>

BackendIntegrator::BackendIntegrator(Logger* logger){
    _logger = logger;
    _esp = new EspWifiClient();
    _timer = new Ticker(BACKEND_REFRESH_INTERVAL_SECONDS * 1000);
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

void BackendIntegrator::addUpdatedEventHandler(BackendWeatherUpdatedEventCallback callback){
    _onUpdateCallback = callback;
}

void BackendIntegrator::addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback)
{
    _onNetworkStatusChangeCallback = callback;
}

void BackendIntegrator::requestWeatherData(){
    if(!connectWifi())return;
    _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

    HttpResponse response = _esp->sendGetRequest(
        SERVER_HOST, String(SERVER_API_GET_OUTDOOR_WEATHER) + "?c=" + String(DATA_COLLECTION_CAPACITY));

    if(response.success && response.statusCode == 200){
        _onNetworkStatusChangeCallback(NetworkStatus::STANDBY);

        DynamicJsonDocument jsonDoc(3000);
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

            _backendWeatherHistoricalData.push_back(wd);
        }

        if(_onUpdateCallback != NULL){
            PresentingBackendWeatherData presentingBackendWeatherData {
                .backendWeatherHistoricalData = _backendWeatherHistoricalData
            };

            if(wd.PM_2_5 > PM2_5_LEVEL_ALERT) presentingBackendWeatherData.PMWarningLevel = WarningLevel::HI_WARNING_LEVEL;
            else if(wd.PM_2_5 > PM2_5_LEVEL_WARNING) presentingBackendWeatherData.PMWarningLevel = WarningLevel::WARNING;
            else if(wd.PM_2_5 != -1)presentingBackendWeatherData.PMWarningLevel = WarningLevel::LOW_WARNING_LEVEL;

            _onUpdateCallback(presentingBackendWeatherData);
        }
    }
    else _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
}

bool BackendIntegrator::connectWifi(){
    if(!IsConnected || !_esp->isWifiConnected()){
        _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

        IsConnected = _esp->connectWifi(WIFI_SSID, WIFI_PASSWORD);

        if(IsConnected) _onNetworkStatusChangeCallback(NetworkStatus::STANDBY);
        else _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
    }

    return IsConnected;
}
void BackendIntegrator::disconnectWifi(){
    _esp->disconnectWifi();
    _onNetworkStatusChangeCallback(NetworkStatus::DISABLED);
}