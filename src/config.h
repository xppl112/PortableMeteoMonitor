#include <Arduino.h>
#include <MCP.h>
#include <SPI.h>

#ifndef CONFIG_H
#define CONFIG_H

const short DEFAULT_MEASUREMENT_DURATION_SECONDS = 30;
const short DEFAULT_CALMDOWN_DURATION_SECONDS = 10;
const int HEALTHCHECK_INTERVAL_MS = 1000;
const int UI_REDRAW_INTERVAL_MS = 1000;
static const uint8_t DATA_COLLECTION_CAPACITY = 30;

static const int CO2_LEVEL_WARNING = 800;
static const int CO2_LEVEL_ALERT = 1400;
static const int PM2_5_LEVEL_WARNING = 36;
static const int PM2_5_LEVEL_ALERT = 56;
static const int CH2O_LEVEL_WARNING = 40;
static const int CH2O_LEVEL_ALERT = 100;

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
const uint8_t BUZZER_PIN = MCP_A1;
const uint8_t LIGHT_SENSOR_PIN = A0;

struct BackendClientConfig {
    const char* WifiSSID = "xppl";
    const char* WifiPassword = "william2015";

    const char* ServerHost = "http://194.58.70.17:5100";
    const char* ServerApiGetTimeUrl = "/api/time";
};


#endif