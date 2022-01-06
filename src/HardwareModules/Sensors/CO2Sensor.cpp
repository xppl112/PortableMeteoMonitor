#include "HardwareModules/Sensors/CO2Sensor.h"

CO2Sensor::CO2Sensor(){
    _sensor = new MHZ19();
}

void CO2Sensor::connect(){
    _sensor->begin(Serial); // *Serial(Stream) refence must be passed to library begin(). 
    _sensor->autoCalibration(); // Turn auto calibration ON (OFF autoCalibration(false))
    _isConnected = true;
}

void CO2Sensor::reset(){

}

CO2SensorData CO2Sensor::getData(){
    CO2SensorData data;
    data.isDataReceived = false;

    if(!_isConnected)
        return data;

    data.concentration = _sensor->getCO2(); 
    data.temperature = _sensor->getTemperature();

    data.isDataReceived = data.concentration >= 1 && data.concentration <= 5000;

    this->registerDataFetching(data.isDataReceived);

    return data;
}