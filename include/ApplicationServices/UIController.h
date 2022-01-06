#include "HardwareModules/HardwareRegistry.h"
#include "UI/LEDIndicatorsController.h"
#include "UI/ScreenController.h"
#include "UI/InputsController.h"
#include "Models/Enums/View.h"
#include "Models/PresentingData.h"
#include "Log4Esp.h"
#include <Ticker.h>

class UIController
{
public:
    UIController(HardwareRegistry* HardwareRegistry, Logger* logger);
    void updateUI();
    void onPresentingDataUpdate(PresentingData presentingData);

private:
    void updateInputs();
    void redrawUI();

    Logger* _logger;
    ScreenController* _screen;
    LEDIndicatorsController* _ledIndicators;
    InputsController* _inputsController;
    Ticker* _timer;

    PresentingData _currentPresentingData;

    View _currentView = View::MAIN_DASHBOARD;
    bool isSoundEnabled = false;
    bool isLedEnabled = true;
};