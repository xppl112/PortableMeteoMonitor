#include <config.h>
#include "Log4Esp.h"
#include "GlobalObjects/GlobalState.h"
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
void onRuntimePreferencesChangedEventHandler(RuntimePreferences runtimePreferences);
void onSourceChangedEventHandler(Source source);

void setup() {
    Serial.begin(9600);

    logger = new Logger("defaultLogger", false);
    hardwareRegistry = new HardwareRegistry(logger);
    hardwareRegistry->reconnectAllDisconnectedDevices();

    weatherMonitor = new WeatherMonitor(hardwareRegistry, logger);
    backendIntegrator = new BackendIntegrator(logger);

    uiController = new UIController(hardwareRegistry, logger);
    uiController->addRuntimePreferencesChangedEventHandler(onRuntimePreferencesChangedEventHandler);
    uiController->addSourceChangedEventHandler(onSourceChangedEventHandler);

    weatherMonitor->addUpdatedEventHandler(onPresentingWeatherDataUpdateEventHandler);
    weatherMonitor->addBlockingEventHandler(onBlockingEventHandler);
    weatherMonitor->run();

    backendIntegrator->addUpdatedEventHandler(onPresentingBackendWeatherDataUpdateEventHandler);
    backendIntegrator->addNetworkStatusChangedEventHandler(onNetworkStatusChangeEventHandler);
    backendIntegrator->addBlockingEventHandler(onBlockingEventHandler);
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

void onRuntimePreferencesChangedEventHandler(RuntimePreferences runtimePreferences){
    
}

void onSourceChangedEventHandler(Source source){
    
}