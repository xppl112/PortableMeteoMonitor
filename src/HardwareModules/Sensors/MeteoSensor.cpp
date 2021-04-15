#include "HardwareModules/Sensors/MeteoSensor.h"

MeteoSensor::MeteoSensor(uint8_t bme_i2c_address){
    _sensor =  new BME280Sensor(bme_i2c_address);
}

void MeteoSensor::connect(){
    _sensor->connect();
}

void MeteoSensor::reset(){

}

MeteoSensorData MeteoSensor::getData(){
    MeteoSensorData data;
    data.isDataReceived = false;

    if(!_sensor->isConnected)
        return data;

    auto bmeData = _sensor->readData();
    this->registerDataFetching(bmeData.isDataReceived);

    data.isDataReceived = bmeData.isDataReceived;
    data.temperatureCelsium = bmeData.temperatureCelsium;
    data.humidityPercent = bmeData.humidityPercent;
    data.pressureInHPascals = bmeData.pressureInHPascals;

    return data;
}