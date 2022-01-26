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
typedef void (*BlockingEventCallback)(bool);

class BackendIntegrator
{
public:
    BackendIntegrator(Logger* logger);
    void run();
    void stop();
    void updateTimers();
    void addUpdatedEventHandler(BackendWeatherUpdatedEventCallback callback){_onUpdateCallback = callback;}
    void addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback){_onNetworkStatusChangeCallback = callback;}
    void addBlockingEventHandler(BlockingEventCallback callback){_onBlockingCallback = callback;}

    bool IsConnected = false;

private:
    void requestWeatherData();
    bool connectWifi();
    void disconnectWifi();

    Logger* _logger;
    Ticker* _timer;
    std::vector<BackendWeatherData> _backendWeatherHistoricalData;

    BackendWeatherUpdatedEventCallback _onUpdateCallback;
    BackendNetworkStatusChangedEventCallback _onNetworkStatusChangeCallback;
    BlockingEventCallback _onBlockingCallback;

    EspWifiClient* _esp;
};
