#ifndef AirParticiplesSensor_H
#define AirParticiplesSensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include <SoftwareSerial.h>
#include "PlantowerSensor.h"

struct AirParticiplesSensorData : public SensorDataBase {
    uint16_t PM_1_0;
    uint16_t PM_2_5;
    uint16_t PM_10_0;
};

class AirParticiplesSensor : public ISensor
{
public:
    AirParticiplesSensor(uint8_t rxPin, uint8_t txPin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "PMS7003",
            .ConnectionType = HardwareModuleConnectionType::UART,
            .PowerVoltage = 5,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 100,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return 2;};

    void connect() override;
    void reset() override;
    bool isConnected() override { _isConnected = _sensor->isConnected; return _isConnected; }
    
    void wakeUp();
    void sleep();
    AirParticiplesSensorData getData();

private:
    PlantowerSensor* _sensor;
    SoftwareSerial *_serial;
};

#endif