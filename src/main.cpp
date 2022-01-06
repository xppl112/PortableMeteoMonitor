#include <config.h>
#include "Log4Esp.h"
#include "ApplicationServices/WeatherMonitor.h"
#include "ApplicationServices/UIController.h"

Logger* logger;
HardwareRegistry* hardwareRegistry;
WeatherMonitor* weatherMonitor;
UIController* uiController;
void onPresentingDataUpdateEventHandler(PresentingData presentingData);

void setup() {
    Serial.begin(9600);

    logger = new Logger("defaultLogger", false);
    hardwareRegistry = new HardwareRegistry(logger);
    hardwareRegistry->reconnectAllDisconnectedDevices();

    uiController = new UIController(hardwareRegistry, logger);
    weatherMonitor = new WeatherMonitor(hardwareRegistry, logger);

    weatherMonitor->addUpdatedEventHandler(onPresentingDataUpdateEventHandler);
    weatherMonitor->run();
}

void loop()
{
    hardwareRegistry->healthCheck();
    uiController->updateUI();
    weatherMonitor->updateTimers();
}

void onPresentingDataUpdateEventHandler(PresentingData presentingData){
    uiController->onPresentingDataUpdate(presentingData);
}