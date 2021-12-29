#include "HardwareModules/HardwareRegistry.h"
#include "UI/LEDIndicatorsController.h"
#include "UI/ScreenController.h"
#include "UI/InputsController.h"
#include "Models/WeatherMonitorData.h"
#include "Log4Esp.h"

class UIController
{
public:
    UIController(HardwareRegistry* HardwareRegistry, Logger* logger);
    void updateUI();
    void onWeatherUpdated(WeatherMonitorData weatherMonitorData);

private:
    enum class ScreenMode {OFF, MAIN_DASHBOARD};
    void updateInputs();
    void redrawUI();

    Logger* _logger;
    ScreenController* _screen;
    LEDIndicatorsController* _ledIndicators;
    InputsController* _inputsController;

    WeatherMonitorData _currentWeather;

    ScreenMode _currentScreenMode = ScreenMode::MAIN_DASHBOARD;
};