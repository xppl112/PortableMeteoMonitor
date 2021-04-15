#include <queue>
#include "Integrations/BackendClient.h"
#include "Healthchecks/HealthcheckController.h"

class BackendIntegrator
{
public:
    BackendIntegrator(HealthcheckController* healthcheckController);
    void updateTimers();
    void onWeatherUpdated(WeatherMonitorData weatherMonitorData);
    void resetModule();

private:
    BackendClient* _backendClient;
    Ticker* _statusReportTimer;
    Ticker* _timeUpdateTimer;
    
    HealthcheckController* _healthcheckController;  
    void sendSystemStatusReport();
    void updateServerTime();
    
    std::queue<WeatherMonitorData> _weatherMonitorDataCollection;
    std::queue<SystemHealthReport> _systemHealthReportsCollection;

    void collectWeatherUpdate(WeatherMonitorData weatherMonitorData);
    void collectSystemReport(SystemHealthReport systemHealthReport);
};
