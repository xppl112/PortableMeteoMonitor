#ifndef CH2OSensor_H
#define CH2OSensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include <SoftwareSerial.h>
#include <ze08_ch2o.h>

struct CH2OSensorData : SensorDataBase {
  unsigned short concentration;
};

class CH2OSensor : public ISensor
{
public:
    CH2OSensor(uint8_t rxPin, uint8_t txPin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "ze08-ch2o",
            .ConnectionType = HardwareModuleConnectionType::UART,
            .PowerVoltage = 5,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 100,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return 5;};

    void connect() override;
    void reset() override;
    bool isConnected() override { return this->_isConnected; }

    CH2OSensorData getData();

private:
    Ze08CH2O *_sensor;
    SoftwareSerial *_serial;
};

#endif