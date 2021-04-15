#ifndef CO2Sensor_H
#define CO2Sensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include "MHZ19.h" 

struct CO2SensorData : SensorDataBase {
  int concentration;
  float temperature;
};

class CO2Sensor : public ISensor
{
public:
    CO2Sensor();
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "MHZ19",
            .ConnectionType = HardwareModuleConnectionType::UART,
            .PowerVoltage = 5,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 200,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return 5;};

    void connect() override;
    void reset() override;
    bool isConnected() override { return this->_isConnected; }

    CO2SensorData getData();

private:
    MHZ19 *_sensor;
};

#endif