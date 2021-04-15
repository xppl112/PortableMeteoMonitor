#include "UI/ScreenController.h"
#include "StringUtils.h"

ScreenController::ScreenController(HardwareRegistry* HardwareRegistry){
    _oled = HardwareRegistry->oledScreen;
}

void ScreenController::clearScreen(){
    _oled->clear();
    _oled->render();
}

void ScreenController::showSplashScreen(){
    _oled->setWordWrapMode(false);
    _oled->clear();
    _oled->setCursor(30,9);_oled->print("Weather", OLEDFont::FONT_SMALL);
    _oled->setCursor(35,25);_oled->print("station", OLEDFont::FONT_SMALL);
    _oled->setCursor(45,50);_oled->print("LOADING...", OLEDFont::FONT_SMALLEST);
    _oled->render();
}

void ScreenController::showIndoorWeather(WeatherMonitorData weatherData){
    _oled->setWordWrapMode(false);
    _oled->clear();

    _oled->setCursor(0,11);
    _oled->print("Indoor weather", OLEDFont::FONT_TITLE);

    _oled->setCursor(0,30);
    _oled->print("temp: " + StringUtils::floatToString(weatherData.temperatureInside) + "`C", OLEDFont::FONT_SMALL);
    _oled->setCursor(0,45);
    _oled->print("hum:  " + String(weatherData.humidityInside) + " %", OLEDFont::FONT_SMALL);
    _oled->render();
}

void ScreenController::showOutdoorTemperature(WeatherMonitorData weatherData){
    _oled->setWordWrapMode(false);
    _oled->clear();

    _oled->setCursor(0,12);
    _oled->print("Outdoor temp", OLEDFont::FONT_TITLE);

    _oled->setCursor(0,55);
    _oled->print(StringUtils::floatToString(weatherData.temperatureOutside) + "`C", OLEDFont::FONT_BODY_BIG);
    _oled->render();
}

void ScreenController::showOutdoorHumidityAndPressure(WeatherMonitorData weatherData){
    _oled->setWordWrapMode(false);
    _oled->clear();

    _oled->setCursor(0,12);
    _oled->print("Outdoor weather", OLEDFont::FONT_TITLE);

    _oled->setCursor(0,30);
    _oled->print("temp: " + StringUtils::floatToString(weatherData.temperatureOutside) + "`C", OLEDFont::FONT_SMALL);
    _oled->setCursor(0,45);
    _oled->print("hum:  " + String(weatherData.humidityOutside) + " %", OLEDFont::FONT_SMALL);
    _oled->setCursor(0,60);
    _oled->print("press: " + StringUtils::floatToString(weatherData.pressureOutside) + " hPa", OLEDFont::FONT_SMALL);
    _oled->render();
}

void ScreenController::showAirQualityMeasurements(WeatherMonitorData weatherData){
    _oled->setWordWrapMode(false);
    _oled->clear();

    _oled->setCursor(0,12);
    _oled->print("PM 2.5", OLEDFont::FONT_TITLE);

    _oled->setCursor(0,55);
    _oled->print(String(weatherData.PM2_5), OLEDFont::FONT_BODY_BIG);
    _oled->render();
}

void ScreenController::showAirPollution(WeatherMonitorData weatherData){
    _oled->setWordWrapMode(false);
    _oled->clear();

    _oled->setCursor(0,11);
    _oled->print("Air pollution AQI", OLEDFont::FONT_TITLE);

    _oled->setCursor(0,55);
    _oled->print(String(weatherData.calculateAQI()), OLEDFont::FONT_BODY_BIG);

    _oled->render();
}


/*
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setTextWrap(true);
  tft.setCursor(30,80);tft.print("Initialization");

*/

/*
#include <Adafruit_GFX.h>     // Core graphics library

  tft.fillRect(0, 0,320, 240, ST77XX_BLACK);
  tft.setCursor(0, 0);tft.print("CO2: ");tft.setCursor(130, 0);tft.print(CO2);
  tft.setCursor(0, 30);tft.print("PM2.5: ");tft.setCursor(130, 30);tft.print(pmsData.PM_2_5);
  tft.setCursor(0, 60);tft.print("CH2O: ");tft.setCursor(130, 60);tft.print(reading);
  
  tft.setCursor(0, 120);tft.print("Temp BME: ");tft.setCursor(180, 120);tft.print(bmeData.temperatureCelsium);
  tft.setCursor(0, 150);tft.print("Temp CO2: ");tft.setCursor(180, 150);tft.print(tempCO2);
  tft.setCursor(0, 180);tft.print("Humidity: ");tft.setCursor(180, 180);tft.print(bmeData.humidityPercent);
  tft.setCursor(0, 210);tft.print("Pressure: ");tft.setCursor(180, 210);tft.print(bmeData.pressureInHPascals);
 
  tft.setCursor(230, 0);tft.print("L:");tft.setCursor(270, 0);tft.print(lightLevel * 100 / 1024);
*/


/*
float p = 3.1415926;


void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}

*/