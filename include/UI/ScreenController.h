#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/TFTScreen.h"
#include "Models/PresentingData.h"
#include "Models/Enums/View.h"
#include "Log4Esp.h"

#include "UI/Screens/DashboardScreen.h"

class ScreenController
{
public:
    ScreenController(HardwareRegistry* hardwareRegistry, Logger* logger);
    void clearScreen();
    void backlight(bool on);
    void showDataScreen(View view, PresentingData data);
    void showMainButtons();

private:
    TFTScreen* _screenDevice;
    Adafruit_ST7789* _screen;
    Logger* _logger;

    DashboardScreen* _dashboardScreen;

    static const uint16_t MainButtonsDividerColor = 0x3186;
    static const uint16_t MainButtonsBackground = 0x3186;
    static const uint16_t MainButtonsDefaultTextColor = 0xAD75;
};