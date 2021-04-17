#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/TFTScreen.h"
#include "Models/AllSensorsData.h"
#include "Log4Esp.h"

class ScreenController
{
public:
    ScreenController(HardwareRegistry* hardwareRegistry, Logger* logger);
    void clearScreen();
    void showSplashScreen();
    void showMainScreen(AllSensorsData data);

private:
    TFTScreen* _screenDevice;
    Adafruit_ST7789* _screen;
    Logger* _logger;
};