#include <config.h>
#include "Log4Esp.h"
#include "HardwareModules/HardwareRegistryFactory.h"
#include "ApplicationServices/WeatherMonitor.h"
#include "ApplicationServices/UIController.h"

Logger* _logger;
HardwareRegistry* _hardwareRegistry;
WeatherMonitor* weatherMonitor;
UIController* uiController;
void onWeatherUpdatedEventHandler(WeatherMonitorData weatherMonitorData);

void setup() {
    Serial.begin(9600);

    _logger = new Logger("defaultLogger", false);
    _hardwareRegistry = HardwareRegistryFactory::createHardwareRegistry();
    _hardwareRegistry->reconnectAllDisconnectedDevices();

    uiController = new UIController(_hardwareRegistry, _logger);
    weatherMonitor = new WeatherMonitor(_hardwareRegistry, _logger);

    weatherMonitor->addUpdatedEventHandler(onWeatherUpdatedEventHandler);
    weatherMonitor->run();
}

void loop()
{
    uiController->updateUI();
    weatherMonitor->updateTimers();
}

void onWeatherUpdatedEventHandler(WeatherMonitorData weatherMonitorData){
    uiController->onWeatherUpdated(weatherMonitorData);
}