#include <Arduino.h>
#include "UI/LEDIndicatorsController.h"
#include "Config.h"

LEDIndicatorsController::LEDIndicatorsController(HardwareRegistry* hardwareRegistry, Logger* logger){
    _logger = logger;
    auto mcpExtender = hardwareRegistry->getDevice<MCPExtender>((HardwareId)HardwareIdList::MCP_EXTENDER);
    if(!mcpExtender->isConnected()){
        _logger->error("mcpExtender is not connected during initializing LEDIndicatorsController");
    }

    _topLed = McpRGBLed(mcpExtender->get(), LED_TOP_RGB_R, LED_TOP_RGB_G, LED_TOP_RGB_B, false);
    _bottomLed = McpRGBLed(mcpExtender->get(), LED_BOTTOM_RGB_R, LED_BOTTOM_RGB_G, LED_BOTTOM_RGB_B, false);

    _timerSlow = new Ticker(1000, NULL, MILLIS); 
    _timerSlow->start();
    _timerFast = new Ticker(400, NULL, MILLIS); 
    _timerFast->start();
}
/*
void LEDIndicatorsController::setPollutionLevel(WeatherMonitorData weatherData){
    if(!weatherData.isPMDataReceived) return;
    
    int aqi = weatherData.calculateAQI();
    if(aqi >= (int)AQILevel::Good && aqi < (int)AQILevel::Moderate) _pollutionRGBLed->setColor(0, 255, 0);
    else if(aqi >= (int)AQILevel::Moderate && aqi < (int)AQILevel::UnhealthyForSensitiveGroups) _pollutionRGBLed->setColor(255, 255, 0);
    else if(aqi >= (int)AQILevel::UnhealthyForSensitiveGroups && aqi < (int)AQILevel::Unhealthy) _pollutionRGBLed->setColor(255, 127, 0);
    else if(aqi >= (int)AQILevel::Unhealthy && aqi < (int)AQILevel::VeryUnhealthy) _pollutionRGBLed->setColor(255, 0, 0);
    else if(aqi >= (int)AQILevel::VeryUnhealthy && aqi < (int)AQILevel::Hazardous) _pollutionRGBLed->setColor(144, 61, 152);
    else if(aqi >= (int)AQILevel::Hazardous) _pollutionRGBLed->setColor(127, 0, 30);
}
*/

void LEDIndicatorsController::setWeatherStatus(AllSensorsData data){  
    _topLed.setColor(RGBLedColor::GREEN);
    _bottomLed.setColor(RGBLedColor::YELLOW);
}

void LEDIndicatorsController::clearAllIndicators(){
    _topLed.setColor(RGBLedColor::BLACK);
    _bottomLed.setColor(RGBLedColor::BLACK);
}