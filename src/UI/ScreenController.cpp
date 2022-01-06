#include "UI/ScreenController.h"
#include "StringUtils.h"
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

void ScreenController::showDataScreen(View view, PresentingData data){
    switch(view){
        case View::MAIN_DASHBOARD:
            _dashboardScreen->show(data);
    }
}