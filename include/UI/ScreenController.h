#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/TFTScreen.h"
#include "Models/PresentingWeatherData.h"
#include "Models/Enums/NetworkStatus.h"
#include "Models/Enums/View.h"
#include "Log4Esp.h"

#include "UI/Screens/DashboardScreen.h"

class ScreenController
{
public:
    ScreenController(HardwareRegistry* hardwareRegistry, Logger* logger);
    void clearScreen();
    void backlight(bool on);
    void showDataScreen(View view, PresentingWeatherData weatherData);
    void showDataScreen(View view, PresentingBackendWeatherData backendWeatherData);    
    void showNetworkStatusIcon(NetworkStatus status);
    void showMainButtons(bool blocked, bool isLedEnabled, bool isSoundEnabled);

private:
    TFTScreen* _screenDevice;
    Adafruit_ST7789* _screen;
    Logger* _logger;

    DashboardScreen* _dashboardScreen;

    static const uint16_t MainButtonsDividerColor = 0x3186;
    static const uint16_t MainButtonsBackground = 0x3186;
    static const uint16_t MainButtonsDefaultTextColor = 0xAD75;
    static const uint16_t MainButtonsDisabledBackground = 0x58E3;
    static const uint16_t MainButtonsDisabledTextColor = 0x8AEB;

    static const uint16_t WifiDisabledIconBackgroundColor = 0x3186;
    static const uint16_t WifiDisabledIconSymbolColor = 0xAD75;
    static const uint16_t WifiProblemIconBackgroundColor = 0x8000;
    static const uint16_t WifiProblemIconSymbolColor = 0xF800;
    static const uint16_t WifiStandByIconBackgroundColor = 0x00EF;
    static const uint16_t WifiStandByIconSymbolColor = 0x5C1F;
    static const uint16_t WifiActiveIconBackgroundColor = 0x03FF;
    static const uint16_t WifiActiveIconSymbolColor = 0xFFFF;
};