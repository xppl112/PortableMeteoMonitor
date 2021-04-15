#include "UI/MenuController.h"
#include "DateTimeUtils.h"
#include "SystemUtils.h"
#include "Config.h"
#include "GlobalObjects/GlobalSystemState.h"
extern GlobalSystemState* globalSystemState;

#define TITLE_Y_POSITION 12
#define BODY_Y_POSITION 28
#define BODY_LINE_HEIGHT 16

MenuController::MenuController(HardwareRegistry* HardwareRegistry, HealthcheckController* healthcheckController){
    _screen = HardwareRegistry->oledScreen;
    _HardwareRegistry = HardwareRegistry;
    _healthcheckController = healthcheckController;
    
    _screenRefreshTimer = new Ticker(MENU_REFRESH_INTERVAL_SECONDS * 1000, NULL, MILLIS);
    _screenRefreshTimer->start();
}

void MenuController::showMenu(MenuMode menuMode){
    _menuMode = menuMode;
    _menuOptionSelectedIndex = 0;
    updateMenu();
}

void MenuController::updateMenu(){
    switch(_menuMode){
        case MenuMode::MENU_DEVICES_INFO:
            showDevicesInfo();
            break;
        case MenuMode::MENU_CURRENT_STATE:
            showCurrentState();
            break;
        case MenuMode::MENU_SYSTEM_INFO:
            showSystemInfo();
            break;
        case MenuMode::MENU_ERRORS_LOG:
            showErrorsLog();
            break;
        case MenuMode::MENU_DEBUG_MODE:
            break;
        case MenuMode::MENU_OFF:
            _screen->clear();
            break;  
        default: printMenu();
    }
}

void MenuController::buttonPressed(ButtonPressed button){
    switch(button){
        case ButtonPressed::LEFT:
            returnBack();
            break;
        case ButtonPressed::RIGHT:
            clickOption();            
            break;
        case ButtonPressed::UP:
            selectPreviousOption();
            break;
        case ButtonPressed::DOWN:
            selectNextOption();
            break;
        case ButtonPressed::LEFTRIGHT:break;
        case ButtonPressed::NONE:break;
    }
}

void MenuController::refresh(){
    _screenRefreshTimer->update();
    if(_screenRefreshTimer->state() == FIRED){
        if(getCurrentScreen().autoRefresh) updateMenu();
        _screenRefreshTimer->resetStatus();
    }   
}

void MenuController::printMenu(){
    MenuScreen menuScreen = getCurrentScreen();
    int selectedOption = _menuOptionSelectedIndex;

    _screen->setWordWrapMode(false);
    _screen->clear();

    _screen->setCursor(0, TITLE_Y_POSITION);
    _screen->print(menuScreen.title, OLEDFont::FONT_TITLE);

    for(int i=0; i< (int)menuScreen.options.size();i++){
        if(selectedOption == i){
            _screen->setCursor(2, BODY_Y_POSITION + i*BODY_LINE_HEIGHT);
            _screen->print("||" + menuScreen.options.at(i).title, OLEDFont::FONT_SMALL);   
        }         
        else {
            _screen->setCursor(10, BODY_Y_POSITION + i*BODY_LINE_HEIGHT);
            _screen->print(menuScreen.options.at(i).title, OLEDFont::FONT_SMALL_THIN);
        }
    }

    _screen->render();
}

void MenuController::showDevicesInfo(){
    auto devicesStatus = _healthcheckController->hardwareDevicesHealthcheck->getDevicesStatus();
    printMenu();

    int lineNumber = -1;
    for (auto module : devicesStatus) {
        lineNumber++;
        if(lineNumber < _scrollerPosition) continue;

        auto device = _HardwareRegistry->getDevice(module.first);
        if(device == NULL)continue;

        _screen->setCursor(0, BODY_Y_POSITION + lineNumber * BODY_LINE_HEIGHT);

        String status;
        switch(module.second){
            case HardwareDeviceStatus::NO_REGISTERED: status = " "; break;
            case HardwareDeviceStatus::CONNECTED: status = "+"; break;
            case HardwareDeviceStatus::DISCONNECTED: status = "-"; break;
            case HardwareDeviceStatus::FAILURE: status = "!"; break;
        }

        _screen->print(status + " " + device->getInfo().Name, OLEDFont::FONT_SMALL_THIN);        
    }
    _screen->render();
}

void MenuController::showCurrentState(){
    printMenu();

    std::vector<String> currentStateVariables;
    currentStateVariables.push_back("Datetime: " + DateTimeUtils::formatFromTimestamp(globalSystemState->getCurrentTimestamp(), TIMEZONE_HOURS_OFFSET));
    currentStateVariables.push_back("NightMode: " + String(globalSystemState->isNightMode? "ON" : "OFF"));
    currentStateVariables.push_back("Unsync data: " + 
                                    String(globalSystemState->unsyncronizedWeatherReports) + " weather reports; " + 
                                    String(globalSystemState->unsyncronizedHealthReports) + " health reports");

    printStringLines(currentStateVariables);
}

void MenuController::showSystemInfo(){
    printMenu();

    std::vector<String> infoCollection;
    infoCollection.push_back("RAM: " + String((int)(SystemUtils::getFreeRAMBytes())) + " / " +
                                    String((int)(SystemUtils::getTotalRAMBytes())));
    infoCollection.push_back("CPU: " + String(SystemUtils::getCPUSpeedMHz()) + " MHz");   
    infoCollection.push_back("Vin: " + String(_healthcheckController->powerHealthcheck->getInputVoltage()) + " V");   
    printStringLines(infoCollection);
}

void MenuController::showErrorsLog(){
    printMenu();
    std::vector<String> errorsStrings;

    for(auto error : globalSystemState->getAllErrors()){
        String errorSeverityStr;
        switch(error.severity){
            case SystemErrorSeverity::SystemError: errorSeverityStr = "!!"; break;
            case SystemErrorSeverity::SystemWarning: errorSeverityStr = "! "; break;
        }

        errorsStrings.push_back(
            errorSeverityStr + " [" + String((int)error.errorCode) + "] "+ error.description);
    }

    printStringLines(errorsStrings);
}

void MenuController::printStringLines(std::vector<String> strings){
    _screen->setWordWrapMode(true);
    int lineNumber = -1;
    int currentCursorPositionY = BODY_Y_POSITION;
    _screen->setCursor(0, BODY_Y_POSITION);

    for(auto line : strings){
        lineNumber++;
        if(lineNumber < _scrollerPosition) continue;

        size_t lineHeight = _screen->print(line, OLEDFont::FONT_SMALLEST);
        currentCursorPositionY += lineHeight + 5;
        _screen->setCursor(0, currentCursorPositionY);
    }

    _screen->render();
}

void MenuController::showDebugScreen(){
    printMenu();
}

void MenuController::selectNextOption(){
    _menuOptionSelectedIndex++;
    if(_menuOptionSelectedIndex >= getCurrentScreenOptionsCount()) _menuOptionSelectedIndex = getCurrentScreenOptionsCount()-1;
    updateMenu();
}

void MenuController::selectPreviousOption(){
    _menuOptionSelectedIndex--;
    if(_menuOptionSelectedIndex < 0) _menuOptionSelectedIndex = 0;
    updateMenu();
}

void MenuController::clickOption(){
    if(getCurrentScreenOptionsCount() == 0) return;
    showMenu(getSelectedOption().nextScreen);
}

void MenuController::returnBack(){
    showMenu(getCurrentScreen().previousScreen);
}