#include <stdint.h>
#include <Wire.h>
#include "Adafruit_BME280.h"

struct BME280Data {
  bool isDataReceived;

  float pressureInHPascals;
  float temperatureCelsium;
  float humidityPercent;
};

class BME280Sensor
{
public:
  BME280Sensor(uint8_t i2c_address);

  bool connect(bool connectionProbe = true, bool waitUntilConnected = false);
  BME280Data readData();

  bool isConnected;

private:
  Adafruit_BME280* _bme;
  uint8_t _i2c_address;
};