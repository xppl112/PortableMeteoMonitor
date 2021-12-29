#include "UI/ScreenController.h"
#include "StringUtils.h"
#include "Config.h"
#include <Adafruit_GFX.h>

ScreenController::ScreenController(HardwareRegistry* hardwareRegistry, Logger* logger){
    _logger = logger;
    _screenDevice = hardwareRegistry->getDevice<TFTScreen>((HardwareId)HardwareIdList::TFT_SCREEN);
    _screen = _screenDevice->get();
    if(!_screenDevice->isConnected()){
        logger->error("TFTScreen is not connected during initialization of ScreenController");
    }
    _screen->setRotation(TFT_ROTATION);
    clearScreen();
}

void ScreenController::clearScreen(){
    _screen->fillScreen(ST77XX_BLACK);
    _screen->setCursor(0, 0);
}

void ScreenController::showSplashScreen(){
    _screen->setTextColor(ST77XX_WHITE);
    _screen->setTextSize(3);
    _screen->setTextWrap(true);
    _screen->setCursor(25,80);
    _screen->print("Initialization");
}

void ScreenController::showMainScreen(WeatherMonitorData data){
    clearScreen();
    _screen->setCursor(0, 0);_screen->print("CO2: ");_screen->setCursor(130, 0);_screen->print(data.CO2);
    _screen->setCursor(0, 30);_screen->print("PM2.5: ");_screen->setCursor(130, 30);_screen->print(data.PM_2_5);
    _screen->setCursor(0, 60);_screen->print("CH2O: ");_screen->setCursor(130, 60);_screen->print(data.CH2O);

    _screen->setCursor(0, 120);_screen->print("Temp BME: ");_screen->setCursor(180, 120);_screen->print(data.temperatureCelsium);
    _screen->setCursor(0, 180);_screen->print("Humidity: ");_screen->setCursor(180, 180);_screen->print(data.humidityPercent);
    _screen->setCursor(0, 210);_screen->print("Pressure: ");_screen->setCursor(180, 210);_screen->print(data.pressureInHPascals);
}


/*
float p = 3.1415926;


void testlines(uint16_t color) {
  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < _screen->width(); x+=6) {
    _screen->drawLine(0, 0, x, _screen->height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < _screen->height(); y+=6) {
    _screen->drawLine(0, 0, _screen->width()-1, y, color);
    delay(0);
  }

  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < _screen->width(); x+=6) {
    _screen->drawLine(_screen->width()-1, 0, x, _screen->height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < _screen->height(); y+=6) {
    _screen->drawLine(_screen->width()-1, 0, 0, y, color);
    delay(0);
  }

  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < _screen->width(); x+=6) {
    _screen->drawLine(0, _screen->height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < _screen->height(); y+=6) {
    _screen->drawLine(0, _screen->height()-1, _screen->width()-1, y, color);
    delay(0);
  }

  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < _screen->width(); x+=6) {
    _screen->drawLine(_screen->width()-1, _screen->height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < _screen->height(); y+=6) {
    _screen->drawLine(_screen->width()-1, _screen->height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  _screen->setCursor(0, 0);
  _screen->setTextColor(color);
  _screen->setTextWrap(true);
  _screen->print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < _screen->height(); y+=5) {
    _screen->drawFastHLine(0, y, _screen->width(), color1);
  }
  for (int16_t x=0; x < _screen->width(); x+=5) {
    _screen->drawFastVLine(x, 0, _screen->height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < _screen->width(); x+=6) {
    _screen->drawRect(_screen->width()/2 -x/2, _screen->height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  _screen->fillScreen(ST77XX_BLACK);
  for (int16_t x=_screen->width()-1; x > 6; x-=6) {
    _screen->fillRect(_screen->width()/2 -x/2, _screen->height()/2 -x/2 , x, x, color1);
    _screen->drawRect(_screen->width()/2 -x/2, _screen->height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < _screen->width(); x+=radius*2) {
    for (int16_t y=radius; y < _screen->height(); y+=radius*2) {
      _screen->fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < _screen->width()+radius; x+=radius*2) {
    for (int16_t y=0; y < _screen->height()+radius; y+=radius*2) {
      _screen->drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  _screen->fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = _screen->width()/2;
  int x = _screen->height()-1;
  int y = 0;
  int z = _screen->width();
  for(t = 0 ; t <= 15; t++) {
    _screen->drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  _screen->fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = _screen->width()-2;
    int h = _screen->height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      _screen->drawRoundRect(x, y, w, h, 5, color);
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
  _screen->setTextWrap(false);
  _screen->fillScreen(ST77XX_BLACK);
  _screen->setCursor(0, 30);
  _screen->setTextColor(ST77XX_RED);
  _screen->setTextSize(1);
  _screen->println("Hello World!");
  _screen->setTextColor(ST77XX_YELLOW);
  _screen->setTextSize(2);
  _screen->println("Hello World!");
  _screen->setTextColor(ST77XX_GREEN);
  _screen->setTextSize(3);
  _screen->println("Hello World!");
  _screen->setTextColor(ST77XX_BLUE);
  _screen->setTextSize(4);
  _screen->print(1234.567);
  delay(1500);
  _screen->setCursor(0, 0);
  _screen->fillScreen(ST77XX_BLACK);
  _screen->setTextColor(ST77XX_WHITE);
  _screen->setTextSize(0);
  _screen->println("Hello World!");
  _screen->setTextSize(1);
  _screen->setTextColor(ST77XX_GREEN);
  _screen->print(p, 6);
  _screen->println(" Want pi?");
  _screen->println(" ");
  _screen->print(8675309, HEX); // print 8,675,309 out in HEX!
  _screen->println(" Print HEX!");
  _screen->println(" ");
  _screen->setTextColor(ST77XX_WHITE);
  _screen->println("Sketch has been");
  _screen->println("running for: ");
  _screen->setTextColor(ST77XX_MAGENTA);
  _screen->print(millis() / 1000);
  _screen->setTextColor(ST77XX_WHITE);
  _screen->print(" seconds.");
}

void mediabuttons() {
  // play
  _screen->fillScreen(ST77XX_BLACK);
  _screen->fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  _screen->fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  _screen->fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  _screen->fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  _screen->fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  _screen->fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  _screen->fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  _screen->fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  _screen->fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}

*/