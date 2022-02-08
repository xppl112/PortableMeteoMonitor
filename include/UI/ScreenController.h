#pragma once

#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/TFTScreen.h"
#include "Models/PresentingWeatherData.h"
#include "Models/Enums/NetworkStatus.h"
#include "Models/Enums/Source.h"

#include "UI/Screens/MixedDataScreen.h"
#include "UI/Screens/LocalDataScreen.h"
#include "UI/Screens/OutWeatherScreen.h"

class ScreenController
{
public:
    ScreenController(HardwareRegistry* hardwareRegistry);
    void clearScreen();
    void backlight(bool on);
    void showDataScreen(Source source, PresentingWeatherData weatherData);
    void showDataScreen(Source source, PresentingBackendWeatherData backendWeatherData);    
    void showNetworkStatusIcon(NetworkStatus status);
    void showMainButtons(bool blocked, bool isLedEnabled, bool isSoundEnabled);
    void clearInterface();

private:
    TFTScreen* _screenDevice;
    Adafruit_ST7789* _screen;

    MixedDataScreen* _mixedDataScreen;
    LocalDataScreen* _localDataScreen;
    OutWeatherScreen* _outWeatherScreen;

    static const uint16_t ScreenBackgroundColor = 0x18E3;

    static const uint16_t WifiDisabledIconBackgroundColor = 0x3186;
    static const uint16_t WifiDisabledIconSymbolColor = 0xAD75;
    static const uint16_t WifiProblemIconBackgroundColor = 0x8000;
    static const uint16_t WifiProblemIconSymbolColor = 0xF800;
    static const uint16_t WifiStandByIconBackgroundColor = 0x00EF;
    static const uint16_t WifiStandByIconSymbolColor = 0x5C1F;
    static const uint16_t WifiActiveIconBackgroundColor = 0x03FF;
    static const uint16_t WifiActiveIconSymbolColor = 0xFFFF;
};