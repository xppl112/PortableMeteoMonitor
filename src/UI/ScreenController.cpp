#include "UI/ScreenController.h"
#include "StringUtils.h"
#include "UI/Screens/CommonDrawFunctions.h"
#include "UI/Screens/Elements/BottomButtons.h"
#include "Config.h"

ScreenController::ScreenController(HardwareRegistry* hardwareRegistry, Logger* logger){
    _logger = logger;
    _screenDevice = hardwareRegistry->_TFTScreen;
    _screen = _screenDevice->get();
    _screen->setRotation(TFT_ROTATION);
    clearScreen();

    _dashboardScreen = new DashboardScreen(_screen);
}

void ScreenController::clearScreen(){
    _screen->fillScreen(ScreenBackgroundColor);
    _screen->fillRect(0,220,320,20,0x0000);
    _screen->setCursor(0, 0);
}

void ScreenController::backlight(bool on){
    _screenDevice->backLight(on);
}

void ScreenController::showDataScreen(Source source, PresentingWeatherData weatherData){
    switch(source){
        case Source::MIXED_DATA:
            _dashboardScreen->showWeatherData(weatherData);
    }
}

void ScreenController::showDataScreen(Source source, PresentingBackendWeatherData backendWeatherData){
    switch(source){
        case Source::MIXED_DATA:
            _dashboardScreen->showBackendWeatherData(backendWeatherData);
    }
}

void ScreenController::showNetworkStatusIcon(NetworkStatus status){
    uint16_t wifiIconBackgroundColor;
    uint16_t wifiIconSymbolColor;

    switch (status)
    {
    case NetworkStatus::STANDBY: 
        wifiIconBackgroundColor = WifiStandByIconBackgroundColor;
        wifiIconSymbolColor = WifiStandByIconSymbolColor;
        break;
    case NetworkStatus::PROBLEM:
        wifiIconBackgroundColor = WifiProblemIconBackgroundColor;
        wifiIconSymbolColor = WifiProblemIconSymbolColor;
        break;
    case NetworkStatus::ACTIVE: 
        wifiIconBackgroundColor = WifiActiveIconBackgroundColor;
        wifiIconSymbolColor = WifiActiveIconSymbolColor;
        break;
    default:
        wifiIconBackgroundColor = WifiDisabledIconBackgroundColor;
        wifiIconSymbolColor = WifiDisabledIconSymbolColor;
    }
    
    _screen->fillRect(0, 228, 13, 13, wifiIconBackgroundColor);
    _screen->setFont(NULL);
    _screen->setTextColor(wifiIconSymbolColor);
    _screen->setCursor(4, 230);
    _screen->print("W");
}

void ScreenController::showMainButtons(bool blocked, bool isLedEnabled, bool isSoundEnabled){
    BottomButtons bottomButtons(_screen);
    bottomButtons.showMainScreenButtons(blocked, isLedEnabled, isSoundEnabled);
}