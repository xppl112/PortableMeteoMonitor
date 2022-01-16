#include "HardwareModules/HardwareRegistry.h"
#include "UI/LEDIndicatorsController.h"
#include "UI/SoundController.h"
#include "UI/ScreenController.h"
#include "UI/InputsController.h"
#include "Models/Enums/View.h"
#include "Models/PresentingWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include "Models/Enums/NetworkStatus.h"
#include "Log4Esp.h"
#include <Ticker.h>

class UIController
{
public:
    UIController(HardwareRegistry* HardwareRegistry, Logger* logger);
    void updateUI();
    void onPresentingWeatherDataUpdate(PresentingWeatherData presentingWeatherData);
    void onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData presentingBackendWeatherData);
    void onNetworkStatusChange(NetworkStatus networkStatus);
    void onBlocking(bool isBlocked);

private:
    void updateInputs();
    void drawInterface();

    void toggleLedEnabling();
    void toggleSoundEnabling();

    Logger* _logger;
    ScreenController* _screen;
    LEDIndicatorsController* _ledIndicators;
    SoundController* _soundController;
    InputsController* _inputsController;
    Ticker* _timer;

    PresentingWeatherData _currentPresentingWeatherData;
    PresentingBackendWeatherData _currentPresentingBackendWeatherData;

    View _currentView = View::MAIN_DASHBOARD;
    bool _isSoundEnabled = false;
    bool _isLedEnabled = false;
    bool _isBlocked = false;
};