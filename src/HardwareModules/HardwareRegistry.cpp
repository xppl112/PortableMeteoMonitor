#include <Arduino.h>
#include "Config.h"
#include "HardwareModules/HardwareRegistry.h"

HardwareRegistry::HardwareRegistry(Logger* logger){    
    _logger = logger;
    _timer = new Ticker(HEALTHCHECK_INTERVAL_MS);
    _timer->start(true);

    initializeDevices();
}

void HardwareRegistry::initializeDevices(){
    _airParticiplesSensor = new AirParticiplesSensor(PMS_RX_PIN, PMS_TX_PIN);
    _CH2OSensor = new CH2OSensor(ZE08CH2O_RX_PIN, ZE08CH2O_TX_PIN);
    _CO2Sensor = new CO2Sensor();
    _meteoSensor = new MeteoSensor(BME_I2C_ADDR);
    _lightSensor = new LightSensor(LIGHT_SENSOR_PIN);
    _MCPExtender = new MCPExtender(MCP_EXTENDER_I2C_ADDR);
    _buzzerModule = new BuzzerModule(_MCPExtender->get(), BUZZER_PIN);

    _TFTScreen = new TFTScreen(TFT_WIDTH, TFT_HEIGHT, TFT_CS, TFT_DC, 
      TFT_RST, TFT_BACKLIGHT, TFT_SPI_MODE, true, true);
    _TFTScreen->useMCP(_MCPExtender->get());
}

void HardwareRegistry::healthCheck(){
    _timer->update();
    if(_timer->state() == FIRED){
        reconnectAllDisconnectedDevices();
        _timer->start();
    }
}

void HardwareRegistry::reconnectAllDisconnectedDevices(){    
    if(!_airParticiplesSensor->isConnected())_airParticiplesSensor->connect();
    if(!_CH2OSensor->isConnected())_CH2OSensor->connect();
    if(!_CO2Sensor->isConnected())_CO2Sensor->connect();
    if(!_meteoSensor->isConnected())_meteoSensor->connect();
    if(!_lightSensor->isConnected())_lightSensor->connect();
    if(!_MCPExtender->isConnected())_MCPExtender->connect();
    if(!_buzzerModule->isConnected())_buzzerModule->connect();
    if(!_TFTScreen->isConnected())_TFTScreen->connect();
}
