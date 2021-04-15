#include "Integrations/BackendClient.h"

BackendClient::BackendClient(BackendClientConfig config, Logger logger){
    _config = config;
    _logger = logger;
    _esp = new EspWifiClient();
}

int BackendClient::GetServerTime(){
    if(connectWifi()){
        HttpResponse response = _esp->sendGetRequest(
            _config.ServerHost, 
            _config.ServerApiGetTimeUrl);
        disconnectWifi();

        if(!response.success){
            _logger.error("Http endpoint connection failed", _config.ServerApiGetTimeUrl);
            return -1;
        }

        if(response.statusCode == 200){
            return atoi(response.payload.c_str());
        }
   
        _logger.error("Http endpoint responded with code " + String(response.statusCode), _config.ServerApiGetTimeUrl);
    }

    return -1;
}
/*
bool BackendClient::SendWeatherData(std::queue<WeatherMonitorData>* weatherDataCollection) {
    if(weatherDataCollection->size() == 0) return true;

    if(connectWifi()){
        for(int batchNum = 0; batchNum < _config.BatchRequestsMaxSize; batchNum++){
            if(weatherDataCollection->size() == 0)break;
            auto weatherData = weatherDataCollection->front();

            if(sendPostJsonRequestWithLogging(
                _config.ServerHost, 
                _config.ServerApiPostWeatherDataUrl,
                weatherData.toJson()))
                {
                    weatherDataCollection->pop();
                }
            yield();
        }
        
        disconnectWifi();
        return true;
    }   
    return false;
}
*/

bool BackendClient::sendPostJsonRequestWithLogging(const char* host, const char* resource, String jsonPayload){
    HttpResponse response = _esp->sendPostJsonRequest(
        host, 
        resource,
        jsonPayload);

    if(!response.success){
        _logger.error("Http endpoint connection failed", resource);
        return false;
    }

    if(response.statusCode != 200){
        _logger.error("Http endpoint responded with code " + String(response.statusCode), resource);
    }

    return response.statusCode == 200;
}

bool BackendClient::connectWifi(){
    bool isConnected = _esp->connectWifi(_config.WifiSSID, _config.WifiPassword);
    if(!isConnected){
        _logger.error("Wifi connection failed", _config.WifiSSID);
    }

    return isConnected;
}

void BackendClient::disconnectWifi(){
    _esp->disconnectWifi();
}