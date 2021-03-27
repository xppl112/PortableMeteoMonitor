#include <Wire.h>             // Wire library (required for I2C devices)
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789



#include "BME280Sensor.h"
#include <ze08_ch2o.h>
 
// ST7789 TFT module connections
#define TFT_RST   -1     // TFT RST pin is connected to NodeMCU pin D8 (GPIO15)
#define TFT_DC    D4     // TFT DC  pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    -1     // TFT CS  pin is directly connected to GND
//#define TFT_BACKLIGHT D8
// initialize ST7789 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

#include <Arduino.h>
#include "MHZ19.h"                                        
#include <SoftwareSerial.h>                                // Remove if using HardwareSerial

#define RX_PIN D6                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN D0                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)

Ze08CH2O *ch2o;                                         // Constructor for library
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial

MHZ19 myMHZ19;    

BME280Sensor *bme280;

unsigned long getDataTimer = 0;


#include "PlantowerSensor.h"
PlantowerSensor *pms;
SoftwareSerial pmsSerial(D3, D8);

#include "MCPExtender.h"
MCPExtender *mcpExtender;
const uint8_t MCP_EXTENDER_I2C_ADDR = 0x20;


float getVoltageFromAnalogPin(uint8_t pin, int r1, int r2){
    float Tvoltage=0.0;
    float Vvalue=0.0,Rvalue=0.0;
    float RatioFactor=(float)r2/(float)(r1+r2);

    for(unsigned int i=0;i<10;i++){
        Vvalue=Vvalue+analogRead(pin);         //Read analog Voltage
        delay(5);                              //ADC stable
    }

    Vvalue=(float)Vvalue/10.0;            //Find average of 10 values
    Serial.print(Vvalue);Serial.print("; ");
    Rvalue=(float)(Vvalue/1023.0)*3.3;      //Convert Voltage in 3.3v factor
    Tvoltage=Rvalue/RatioFactor;          //Find original voltage by multiplying with factor

    return Tvoltage;
}


#include "EspWifiClient.h"
EspWifiClient* _esp;
    const char* WifiSSID = "xppl";
    const char* WifiPassword = "william2015";

    const char* ServerHost = "http://194.58.70.17:5100";
    const char* ServerApiGetTimeUrl = "/api/time";
    const char* ServerApiPostWeatherDataUrl = "/api/weather";
    const char* ServerApiPostHealthStatusUrl = "/api/status";


bool connectWifi(){
    bool isConnected = _esp->connectWifi(WifiSSID, WifiPassword);   
    return isConnected;
}

void disconnectWifi(){
    _esp->disconnectWifi();
}

int GetServerTime(){
    if(connectWifi()){
        HttpResponse response = _esp->sendGetRequest(
            ServerHost, 
            ServerApiGetTimeUrl);
        disconnectWifi();    
        return atoi(response.payload.c_str());

    }

    return -1;
}

long GetWifiSignalLevel(){
    if(connectWifi()){
        long rssi = WiFi.RSSI();
        disconnectWifi();    
        return rssi;
    }

    return -1;
}

const int MAX_VAL = -20; // define maximum signal strength (in dBm)
const int MIN_VAL = -80; // define minimum signal strength (in dBm)

const uint8_t DISPLAY_RST_PIN = MCP_B5;
const uint8_t DISPLAY_BLK_PIN = MCP_B4;

void setup(void) {
  Serial.begin(9600);

  mcpExtender = new MCPExtender(MCP_EXTENDER_I2C_ADDR);
  mcpExtender->connect();

  tft.initSPI(0, SPI_MODE2);

    // Toggle _rst low to reset
    mcpExtender->pinMode(DISPLAY_RST_PIN, OUTPUT);
    mcpExtender->digitalWrite(DISPLAY_RST_PIN, HIGH);
    delay(100);
    mcpExtender->digitalWrite(DISPLAY_RST_PIN, LOW);
    delay(100);
    mcpExtender->digitalWrite(DISPLAY_RST_PIN, HIGH);
    delay(200);

  tft.init(240, 320, SPI_MODE2);   

  pinMode(DISPLAY_BLK_PIN, OUTPUT);
  digitalWrite(DISPLAY_BLK_PIN, HIGH); // Backlight on
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setTextWrap(true);
  tft.setCursor(30,80);tft.print("Initialization");

  mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start   
  ch2o = new Ze08CH2O{&mySerial};

  myMHZ19.begin(Serial);                                // *Serial(Stream) refence must be passed to library begin(). 
  myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))

  pmsSerial.begin(9600);
  pms = new PlantowerSensor(&pmsSerial);
  pms->connect();

  _esp = new EspWifiClient();

  bme280 = new BME280Sensor(0x76);
  bme280->connect();

  WiFi.forceSleepBegin();
  delay(1);
}

void loop()
{
/*auto vcc = getVoltageFromAnalogPin(A0, 19400, 9980);
Serial.println(vcc);
*/

auto lightLevel = analogRead(A0);

/*
auto rssi = GetWifiSignalLevel();
int strength = map(rssi, MIN_VAL, MAX_VAL, 0, 11);
*/

 int CO2 = myMHZ19.getCO2(); 
 float tempCO2 = myMHZ19.getTemperature();


  Ze08CH2O::concentration_t reading;
     mySerial.listen();
    ch2o->read(reading);

    pmsSerial.listen();
    auto pmsData = pms->readDataSyncronioslyAndSleep();

    auto bmeData = bme280->readData();


  tft.fillRect(0, 0,320, 240, ST77XX_BLACK);
  tft.setCursor(0, 0);tft.print("CO2: ");tft.setCursor(130, 0);tft.print(CO2);
  tft.setCursor(0, 30);tft.print("PM2.5: ");tft.setCursor(130, 30);tft.print(pmsData.PM_2_5);
  tft.setCursor(0, 60);tft.print("CH2O: ");tft.setCursor(130, 60);tft.print(reading);
  
  tft.setCursor(0, 120);tft.print("Temp BME: ");tft.setCursor(180, 120);tft.print(bmeData.temperatureCelsium);
  tft.setCursor(0, 150);tft.print("Temp CO2: ");tft.setCursor(180, 150);tft.print(tempCO2);
  tft.setCursor(0, 180);tft.print("Humidity: ");tft.setCursor(180, 180);tft.print(bmeData.humidityPercent);
  tft.setCursor(0, 210);tft.print("Pressure: ");tft.setCursor(180, 210);tft.print(bmeData.pressureInHPascals);
 
  tft.setCursor(230, 0);tft.print("L:");tft.setCursor(270, 0);tft.print(lightLevel * 100 / 1024);

    delay(10000);
}















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