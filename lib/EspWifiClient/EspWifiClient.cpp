#include "EspWifiClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

EspWifiClient::EspWifiClient(){
    _wifiStatus = WL_IDLE_STATUS;
}

bool EspWifiClient::connectWifi(const char* ssid, const char* password, uint8_t timeoutSeconds){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    _wifiStatus = WiFi.waitForConnectResult(timeoutSeconds * 1000);

    return isWifiConnected();
}

void EspWifiClient::disconnectWifi() {
    _wifiStatus = WiFi.disconnect(true);
}

bool EspWifiClient::isWifiConnected() {
    return WiFi.isConnected();
}

HttpResponse EspWifiClient::sendGetRequest(String host, String resource, uint8_t timeoutSeconds){
    HttpResponse httpResponse;

    HTTPClient client;
    client.begin(_wifiClient, host + resource);
    client.setTimeout(timeoutSeconds * 1000);

    int resultCode = client.GET();

    if(resultCode != -1){
        httpResponse.statusCode = resultCode;
        
        if(resultCode == HTTP_CODE_OK){
            httpResponse.payload = client.getString();
        }
        else {
            httpResponse.payload = client.errorToString(resultCode).c_str();
        }        
    }

    client.end();

    httpResponse.success = resultCode != -1 ? true : false;
    return httpResponse;
}

HttpResponse EspWifiClient::sendPostJsonRequest(String host, String resource, String jsonPayload, uint8_t timeoutSeconds){
    HttpResponse httpResponse;

    HTTPClient client;
    client.begin(_wifiClient, host + resource);
    client.addHeader("Content-Type", "application/json");
    client.setTimeout(timeoutSeconds * 1000);    

    int resultCode = client.POST(jsonPayload);
    if(resultCode != -1){
        httpResponse.statusCode = resultCode;
        
        if(resultCode == HTTP_CODE_OK){
            httpResponse.payload = client.getString();
        }
        else {
            httpResponse.payload = client.errorToString(resultCode).c_str();
        }        
    }

    client.end();

    httpResponse.success = resultCode != -1 ? true : false;
    return httpResponse;
}