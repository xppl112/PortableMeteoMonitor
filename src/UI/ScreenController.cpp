#include "UI/ScreenController.h"
#include "StringUtils.h"
#include "UI/Screens/CommonDrawFunctions.h"
#include "UI/Screens/Elements/BottomButtons.h"
#include "Config.h"

ScreenController::ScreenController(HardwareRegistry* hardwareRegistry){
    _screenDevice = hardwareRegistry->_TFTScreen;
    _screen = _screenDevice->get();
    _screen->setRotation(TFT_ROTATION);
    clearScreen();

    _mixedDataScreen = new MixedDataScreen(_screen);
    _localDataScreen = new LocalDataScreen(_screen);
    _outWeatherScreen = new OutWeatherScreen(_screen);
}

void ScreenController::clearScreen(){
    _screen->fillScreen(0x0000);    
    clearInterface();
}

void ScreenController::backlight(bool on){
    _screenDevice->backLight(on);
}

void ScreenController::showDataScreen(Source source, PresentingWeatherData weatherData){
    switch(source){
        case Source::MIXED_DATA:
            _mixedDataScreen->showWeatherData(weatherData);break;
        case Source::LOCAL_DATA:
            _localDataScreen->showWeatherData(weatherData);break;
        case Source::OUT_WEATHER:
            break;
        case Source::SOLAR_STATION:
            clearScreen();
            break;
    }
}

void ScreenController::showDataScreen(Source source, PresentingBackendWeatherData backendWeatherData){
    switch(source){
        case Source::MIXED_DATA:
            _mixedDataScreen->showBackendWeatherData(backendWeatherData);break;
        case Source::LOCAL_DATA:
            break;
        case Source::OUT_WEATHER:
            _outWeatherScreen->showBackendWeatherData(backendWeatherData);break;
        case Source::SOLAR_STATION:
            clearScreen();
            break;
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

void ScreenController::clearInterface(){
    _screen->fillRect(159,0,2,220,ScreenBackgroundColor);
    _screen->fillRect(0,72,320,2,ScreenBackgroundColor);
    _screen->fillRect(0,146,320,2,ScreenBackgroundColor);
    _screen->fillRect(15,220,305,20,0x0000);
}