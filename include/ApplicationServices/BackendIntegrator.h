#include "HardwareModules/HardwareRegistry.h"
#include "Models/BackendWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include "Models/Enums/NetworkStatus.h"
#include <Ticker.h>
#include "Log4Esp.h"
#include <vector>
#include "EspWifiClient.h"
#include "config.h"

typedef void (*BackendWeatherUpdatedEventCallback)(PresentingBackendWeatherData);
typedef void (*BackendNetworkStatusChangedEventCallback)(NetworkStatus);

class BackendIntegrator
{
public:
    BackendIntegrator(Logger* logger);
    void run();
    void stop();
    void updateTimers();
    void addUpdatedEventHandler(BackendWeatherUpdatedEventCallback callback);
    void addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback);

    bool IsConnected = false;

private:
    void requestWeatherData();
    bool connectWifi();
    void disconnectWifi();

    Logger* _logger;
    BackendWeatherUpdatedEventCallback _onUpdateCallback;
    BackendNetworkStatusChangedEventCallback _onNetworkStatusChangeCallback;
    Ticker* _timer;
    std::vector<BackendWeatherData> _backendWeatherHistoricalData;

    EspWifiClient* _esp;
};
