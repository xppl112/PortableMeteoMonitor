#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "Arduino.h"

AirParticiplesSensor::AirParticiplesSensor(uint8_t rxPin, uint8_t txPin){
    _serial = new SoftwareSerial(rxPin, txPin); 
    _sensor = new PlantowerSensor(_serial);
}

void AirParticiplesSensor::connect(){
    _serial->begin(9600);
    _serial->listen();
    _isConnected = _sensor->connect();
}

void AirParticiplesSensor::reset(){
    _serial->end();
    connect();
}

void AirParticiplesSensor::beginMeasurement(){
    if(!_sensor->isConnected)
        return;

    _sensor->wakeUp();
}

AirParticiplesSensorData AirParticiplesSensor::endMeasurement(){
    _serial->listen();
    auto pmsData = _sensor->readData();
    this->registerDataFetching(pmsData.isDataReceived);

    AirParticiplesSensorData data;
    data.isDataReceived = pmsData.isDataReceived;
    data.PM_1_0 = pmsData.PM_1_0;
    data.PM_2_5 = pmsData.PM_2_5;
    data.PM_10_0 = pmsData.PM_10_0;

    _sensor->sleep();
    return data;
}