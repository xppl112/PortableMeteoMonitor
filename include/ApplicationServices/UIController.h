#include "HardwareModules/HardwareRegistry.h"
#include "UI/LEDIndicatorsController.h"
#include "UI/SoundController.h"
#include "UI/ScreenController.h"
#include "UI/InputsController.h"
#include "UI/MenuController.h"
#include "Models/Enums/View.h"
#include "Models/RuntimePreferences.h"
#include "Models/Enums/Source.h"
#include "Models/PresentingWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include "Models/Enums/NetworkStatus.h"
#include <Ticker.h>

typedef void (*RuntimePreferencesChangedEventCallback)(RuntimePreferences);
typedef void (*SourceChangedEventCallback)(Source);

class UIController
{
public:
    UIController(HardwareRegistry* HardwareRegistry);
    void updateUI();
    void onPresentingWeatherDataUpdate(PresentingWeatherData presentingWeatherData);
    void onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData presentingBackendWeatherData);
    void onNetworkStatusChange(NetworkStatus networkStatus);
    void onBlocking(bool isBlocked);
    
    void addRuntimePreferencesChangedEventHandler(RuntimePreferencesChangedEventCallback callback){_onRuntimePreferencesChangedCallback = callback;}
    void addSourceChangedEventHandler(SourceChangedEventCallback callback){_onSourceChangedCallback = callback;}

private:
    void updateInputs();
    void drawMainButtons();
    void clearInterface();

    void toggleLedEnabling();
    void toggleSoundEnabling();
    void showMenuScreen(Menu menu);  
    void showDataScreen();  
    void applyMenuChanges(uint8_t selectedOption);  
    void setLightsMode(bool standardLight);

    Ticker* _timer;

    ScreenController* _screen;
    LEDIndicatorsController* _ledIndicators;
    SoundController* _soundController;
    InputsController* _inputsController;
    MenuController* _menuController;

    PresentingWeatherData _currentPresentingWeatherData;
    PresentingBackendWeatherData _currentPresentingBackendWeatherData;

    RuntimePreferencesChangedEventCallback _onRuntimePreferencesChangedCallback;
    SourceChangedEventCallback _onSourceChangedCallback;

    volatile View _currentView = View::MAIN_SCREEN;
    volatile Source _currentSource = Source::MIXED_DATA;
    RuntimePreferences _currentRuntimePreferences;
    bool _isSoundEnabled = false;
    bool _isLedEnabled = false;
    bool _isBlocked = false;

    unsigned long _lastInteractionTimestamp = 0;
    bool _noLightsMode = false;
};