#include "UI/ScreenController.h"
#include "StringUtils.h"
#include "UI/Screens/CommonDrawFunctions.h"
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
    _screen->fillScreen(ST77XX_BLACK);
    _screen->setCursor(0, 0);
}

void ScreenController::backlight(bool on){
    _screenDevice->backLight(on);
}

void ScreenController::showDataScreen(View view, PresentingWeatherData weatherData){
    switch(view){
        case View::MAIN_DASHBOARD:
            _dashboardScreen->showWeatherData(weatherData);
    }
}

void ScreenController::showDataScreen(View view, PresentingBackendWeatherData backendWeatherData){
    switch(view){
        case View::MAIN_DASHBOARD:
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
    _screen->drawLine(0, 225, 320, 225, MainButtonsDividerColor);
    uint16_t buttonBackgroundColor = blocked ? MainButtonsDisabledBackground : MainButtonsBackground;
    uint16_t buttonTextColor = blocked ? MainButtonsDisabledTextColor : MainButtonsDefaultTextColor;

    _screen->fillRect(15, 228, 64, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, "mode", 6, buttonTextColor,15,79,230);

    _screen->fillRect(81, 228, 77, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, "source", 6, buttonTextColor,81,158,230);

    _screen->fillRect(160, 228, 79, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, String("led ") + (isLedEnabled ? "on" : "off"), 6, buttonTextColor,160,239,230);

    _screen->fillRect(241, 228, 79, 13, buttonBackgroundColor);
    CommonDrawFunctions::drawCenteredText(
        _screen, String("sound ") + (isSoundEnabled ? "on" : "off"), 6, buttonTextColor,241,320,230);
}