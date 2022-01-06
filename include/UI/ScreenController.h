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

private:
    TFTScreen* _screenDevice;
    Adafruit_ST7789* _screen;
    Logger* _logger;

    DashboardScreen* _dashboardScreen;
};