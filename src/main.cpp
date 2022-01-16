#include <config.h>
#include "Log4Esp.h"
#include "ApplicationServices/WeatherMonitor.h"
#include "ApplicationServices/BackendIntegrator.h"
#include "ApplicationServices/UIController.h"

Logger* logger;
HardwareRegistry* hardwareRegistry;
WeatherMonitor* weatherMonitor;
BackendIntegrator* backendIntegrator;
UIController* uiController;

void onPresentingWeatherDataUpdateEventHandler(PresentingWeatherData PresentingWeatherData);
void onPresentingBackendWeatherDataUpdateEventHandler(PresentingBackendWeatherData PresentingBackendWeatherData);
void onNetworkStatusChangeEventHandler(NetworkStatus networkStatus);
void onBlockingEventHandler(bool isBlocked);

void setup() {
    Serial.begin(9600);

    logger = new Logger("defaultLogger", false);
    hardwareRegistry = new HardwareRegistry(logger);
    hardwareRegistry->reconnectAllDisconnectedDevices();

    uiController = new UIController(hardwareRegistry, logger);
    weatherMonitor = new WeatherMonitor(hardwareRegistry, logger);
    backendIntegrator = new BackendIntegrator(logger);

    weatherMonitor->addUpdatedEventHandler(onPresentingWeatherDataUpdateEventHandler);
    weatherMonitor->addBlockingEventHandler(onBlockingEventHandler);
    weatherMonitor->run();

    backendIntegrator->addUpdatedEventHandler(onPresentingBackendWeatherDataUpdateEventHandler);
    backendIntegrator->addNetworkStatusChangedEventHandler(onNetworkStatusChangeEventHandler);
    backendIntegrator->run();
}

void loop()
{
    hardwareRegistry->healthCheck();
    uiController->updateUI();
    weatherMonitor->updateTimers();
    backendIntegrator->updateTimers();
}

void onPresentingWeatherDataUpdateEventHandler(PresentingWeatherData PresentingWeatherData){
    uiController->onPresentingWeatherDataUpdate(PresentingWeatherData);
}

void onPresentingBackendWeatherDataUpdateEventHandler(PresentingBackendWeatherData PresentingBackendWeatherData){
    uiController->onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData);
}

void onNetworkStatusChangeEventHandler(NetworkStatus networkStatus){
    uiController->onNetworkStatusChange(networkStatus);
}

void onBlockingEventHandler(bool isBlocked){
    uiController->onBlocking(isBlocked);
}