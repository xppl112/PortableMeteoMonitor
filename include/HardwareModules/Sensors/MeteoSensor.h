#ifndef MeteoSensor_H
#define MeteoSensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include "BME280Sensor.h"

struct MeteoSensorData : SensorDataBase {
  float pressureInHPascals;
  float temperatureCelsium;
  float humidityPercent;
};

class MeteoSensor : public ISensor
{
public:
    MeteoSensor(uint8_t bme_i2c_address);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "BME280",
            .ConnectionType = HardwareModuleConnectionType::I2C,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 0,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return 2;};

    void connect() override;
    void reset() override;
    bool isConnected() override { _isConnected = _sensor->isConnected; return _isConnected; }

    MeteoSensorData getData();

private:
    BME280Sensor* _sensor;
};

#endif