#include <Arduino.h>
#include <MCP.h>
#include <SPI.h>

#ifndef CONFIG_H
#define CONFIG_H

enum class HardwareIdList {
    MCP_EXTENDER, 
    TFT_SCREEN,
    AIR_PARTICIPLES_SENSOR, 
    CH2O_SENSOR, 
    CO2_SENSOR, 
    METEO_SENSOR,
    LIGHT_SENSOR};

// I2C
const uint8_t BME_I2C_ADDR = 0x76;
const uint8_t MCP_EXTENDER_I2C_ADDR = 0x20;

// ST7789 TFT module connections
const uint8_t TFT_DC = D4;
const uint8_t TFT_CS = -1; // TFT CS  pin is directly connected to GND
const uint8_t TFT_SPI_MODE = SPI_MODE2;

const uint8_t TFT_RST = MCP_B5;
const uint8_t TFT_BACKLIGHT = MCP_B4;

const int TFT_WIDTH = 240;
const int TFT_HEIGHT = 320;
const uint8_t TFT_ROTATION = 3;

// Ze08CH2O module UART
const uint8_t ZE08CH2O_RX_PIN = D6; 
const uint8_t ZE08CH2O_TX_PIN = D0;
const int ZE08CH2O_BAUDRATE = 9600;

// PMS module UART
const uint8_t PMS_RX_PIN = D3; 
const uint8_t PMS_TX_PIN = D8;
const int PMS_BAUDRATE = 9600;

// LED
const uint8_t LED_TOP_RGB_R = MCP_A2;
const uint8_t LED_TOP_RGB_G = MCP_B0;
const uint8_t LED_TOP_RGB_B = MCP_B1;
const uint8_t LED_BOTTOM_RGB_R = MCP_A3;
const uint8_t LED_BOTTOM_RGB_G = MCP_B2;
const uint8_t LED_BOTTOM_RGB_B = MCP_B3;

// BUTTONS
const uint8_t TOUCH_BUTTON_PIN = MCP_A5;
const uint8_t LEFT_BUTTON_PIN = MCP_A7;
const uint8_t CENTER_BUTTON_PIN = MCP_A6;
const uint8_t RIGHT_BUTTON_PIN = MCP_A4;

// MISC DEVICES
const uint8_t BUZZER_PIN = MCP_B7;//>????
const uint8_t LIGHT_SENSOR_PIN = A0;

struct BackendClientConfig {
    const char* WifiSSID = "xppl";
    const char* WifiPassword = "william2015";

    const char* ServerHost = "http://194.58.70.17:5100";
    const char* ServerApiGetTimeUrl = "/api/time";
};


#endif