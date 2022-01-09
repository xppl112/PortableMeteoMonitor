#include "HardwareModules/HardwareRegistry.h"
#include "Models/WeatherMonitorData.h"
#include "Models/PresentingData.h"
#include <Ticker.h>
#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "HardwareModules/Sensors/CH2OSensor.h"
#include "HardwareModules/Sensors/CO2Sensor.h"
#include "HardwareModules/Sensors/MeteoSensor.h"
#include "Log4Esp.h"
#include <vector>

typedef void (*WeatherMonitorUpdatedEventCallback)(PresentingData);

class WeatherMonitor
{
public:
    WeatherMonitor(HardwareRegistry* hardwareRegistry, Logger* logger);
    void run();
    void updateTimers();
    void addUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback);
    void reconnectSensors();
    void resetSensors();
    enum class WeatherMonitorState {DISABLED, IDLE, MEASURING};
    WeatherMonitorState state = WeatherMonitorState::DISABLED;

private:
    void startMeasuring();
    void finishMeasuring(bool runWithoutStart = false);
    void registerWeatherData(WeatherMonitorData data);

    Logger* _logger;

    WeatherMonitorUpdatedEventCallback _onUpdateCallback;
    Ticker* _timer;
    unsigned long _startMeasuringTimestamp;

    AirParticiplesSensor* _airParticiplesSensor;
    CH2OSensor* _CH2OSensor;
    CO2Sensor* _CO2Sensor;
    MeteoSensor* _meteoSensor;

    std::vector<WeatherMonitorData> _weatherMonitorHistoricalData;
};
