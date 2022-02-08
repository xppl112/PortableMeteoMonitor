#include "HardwareModules/HardwareRegistry.h"
#include "Models/WeatherMonitorData.h"
#include "Models/PresentingWeatherData.h"
#include "Models/Enums/Mode.h"
#include <Ticker.h>
#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "HardwareModules/Sensors/CH2OSensor.h"
#include "HardwareModules/Sensors/CO2Sensor.h"
#include "HardwareModules/Sensors/MeteoSensor.h"
#include <vector>
#include "config.h"

typedef void (*WeatherMonitorUpdatedEventCallback)(PresentingWeatherData);
typedef void (*BlockingEventCallback)(bool);

class WeatherMonitor
{
public:
    WeatherMonitor(HardwareRegistry* hardwareRegistry);
    void run();
    void stop();
    void updateTimers();
    void addUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback){_onUpdateCallback = callback;}
    void addBlockingEventHandler(BlockingEventCallback callback){_onBlockingCallback = callback;}
    void reconnectSensors();
    void resetSensors();
    void setMeasurementMode(Mode mode);

    enum class WeatherMonitorState {DISABLED, IDLE, MEASURING};
    WeatherMonitorState state = WeatherMonitorState::DISABLED;

private:
    void startMeasuring();
    void finishMeasuring();
    void registerWeatherData(WeatherMonitorData data);

    WeatherMonitorUpdatedEventCallback _onUpdateCallback;
    BlockingEventCallback _onBlockingCallback;
    Ticker* _timer;
    unsigned long _startMeasuringTimestamp;

    AirParticiplesSensor* _airParticiplesSensor;
    CH2OSensor* _CH2OSensor;
    CO2Sensor* _CO2Sensor;
    MeteoSensor* _meteoSensor;

    std::vector<WeatherMonitorData> _weatherMonitorHistoricalData;

    uint16_t _measurementDurationSeconds = DEFAULT_MEASUREMENT_DURATION_SECONDS;
    uint16_t _calmdownDurationSeconds = DEFAULT_CALMDOWN_DURATION_SECONDS;
};
