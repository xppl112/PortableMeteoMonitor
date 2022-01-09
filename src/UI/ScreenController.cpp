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

void ScreenController::showDataScreen(View view, PresentingData data){
    switch(view){
        case View::MAIN_DASHBOARD:
            _dashboardScreen->show(data);
    }

    showMainButtons();
}

void ScreenController::showMainButtons(){
    _screen->drawLine(0, 225, 320, 225, MainButtonsDividerColor);

    _screen->fillRect(0, 228, 79, 13, MainButtonsBackground);
    CommonDrawFunctions::drawCenteredText(
        _screen, "view", 6, MainButtonsDefaultTextColor,0,79,230);

    _screen->fillRect(81, 228, 77, 13, MainButtonsBackground);
    CommonDrawFunctions::drawCenteredText(
        _screen, "mode", 6, MainButtonsDefaultTextColor,81,158,230);

    _screen->fillRect(160, 228, 79, 13, MainButtonsBackground);
    CommonDrawFunctions::drawCenteredText(
        _screen, "led on", 6, MainButtonsDefaultTextColor,160,239,230);

    _screen->fillRect(241, 228, 79, 13, MainButtonsBackground);
    CommonDrawFunctions::drawCenteredText(
        _screen, "sound off", 6, MainButtonsDefaultTextColor,241,320,230);
}