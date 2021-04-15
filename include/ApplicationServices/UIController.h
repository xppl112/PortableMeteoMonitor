#include "HardwareModules/HardwareRegistry.h"
#include "UI/LEDIndicatorsController.h"
#include "UI/MenuController.h"
#include "UI/ScreenController.h"
#include "UI/InputsController.h"
#include "UI/LEDDisplayController.h"
#include "Models/WeatherMonitorData.h"
#include "Healthchecks/HealthcheckController.h"

class UIController
{
public:
    UIController(HardwareRegistry* HardwareRegistry, HealthcheckController* healthcheckController);
    void updateUI();
    void onWeatherUpdated(WeatherMonitorData weatherMonitorData);
    void enableMenuMode(bool debugMode = false);
    void disableMenuMode();

private:
    enum class ScreenMode {OFF, TEMPERATURE_OUTSIDE, AIR_POLLUTION, METEO_OUTSIDE, METEO_INSIDE, AIR_QULITY};
    enum class LedDisplayMode {OFF, TEMPERATURE_OUTSIDE, AIR_POLLUTION};
    void updateInputs();
    void redrawUI();
    void flipScreenMode(bool forward);

    MenuController* _menuController;
    ScreenController* _screen;
    LEDDisplayController* _ledDisplayController;
    LEDIndicatorsController* _ledIndicators;
    InputsController* _inputsController;

    WeatherMonitorData _currentWeather;
    bool _isMenuMode = false;
    unsigned long _lastInteractionTimestamp = 0;
    bool _isInteraction = false;
    void onInteraction();
    void updateOutputDevicesStatus();

    ScreenMode _currentScreenMode = ScreenMode::AIR_POLLUTION;
    LedDisplayMode _currentLedDisplayMode = LedDisplayMode::TEMPERATURE_OUTSIDE;
    bool _isScreenActive = true;
    bool _isLedDisplayActive = true;
};