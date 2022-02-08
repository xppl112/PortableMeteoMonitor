#include <config.h>
#include "GlobalObjects/GlobalState.h"
#include "ApplicationServices/WeatherMonitor.h"
#include "ApplicationServices/BackendIntegrator.h"
#include "ApplicationServices/UIController.h"

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

    hardwareRegistry = new HardwareRegistry();
    hardwareRegistry->reconnectAllDisconnectedDevices();

    weatherMonitor = new WeatherMonitor(hardwareRegistry);
    backendIntegrator = new BackendIntegrator();

    uiController = new UIController(hardwareRegistry);
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
    weatherMonitor->setMeasurementMode(runtimePreferences.mode);
    backendIntegrator->setRefreshMode(runtimePreferences.mode);
}

void onSourceChangedEventHandler(Source source){
    if(source == Source::LOCAL_DATA)backendIntegrator->stop();
    else backendIntegrator->run();

    if(source == Source::OUT_WEATHER)weatherMonitor->stop();
    else weatherMonitor->run();
}