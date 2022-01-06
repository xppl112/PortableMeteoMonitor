#include "UI/Screens/DashboardScreen.h"
#include "StringUtils.h"
#include <Adafruit_GFX.h>

DashboardScreen::DashboardScreen(Adafruit_ST7789* screen){
    _screen = screen;
}

void DashboardScreen::show(WeatherMonitorData data){
    _screen->fillScreen(ST77XX_BLACK);

    _screen->setTextColor(ST77XX_WHITE);
    _screen->setTextSize(3);
    _screen->setTextWrap(true);

    _screen->setCursor(0, 0);_screen->print("CO2: ");_screen->setCursor(130, 0);_screen->print(data.CO2);
    _screen->setCursor(0, 30);_screen->print("PM2.5: ");_screen->setCursor(130, 30);_screen->print(data.PM_2_5);
    _screen->setCursor(0, 60);_screen->print("CH2O: ");_screen->setCursor(130, 60);_screen->print(data.CH2O);

    _screen->setCursor(0, 120);_screen->print("Temp BME: ");_screen->setCursor(180, 120);_screen->print(data.temperatureCelsium);
    _screen->setCursor(0, 180);_screen->print("Humidity: ");_screen->setCursor(180, 180);_screen->print(data.humidityPercent);
    _screen->setCursor(0, 210);_screen->print("Pressure: ");_screen->setCursor(180, 210);_screen->print(data.pressureInHPascals);
}
