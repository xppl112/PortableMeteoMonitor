#include "HardwareModules/Sensors/CH2OSensor.h"

CH2OSensor::CH2OSensor(uint8_t rxPin, uint8_t txPin){
    _serial = new SoftwareSerial(rxPin, txPin);  
    _sensor = new Ze08CH2O{_serial};
}

void CH2OSensor::connect(){
    _serial->begin(9600);
    _isConnected = true;
}

void CH2OSensor::reset(){
    _serial->end();
    connect();
}

CH2OSensorData CH2OSensor::getData(){
    CH2OSensorData data;
    data.isDataReceived = false;

    if(!_isConnected)return data;

    Ze08CH2O::concentration_t concentration;
    _serial->listen();
    _serial->begin(9600);

    if(_sensor->getGasConcentration(concentration)){
        data.concentration = concentration;

        if(data.concentration >= 0 && data.concentration <= 5000)
            data.isDataReceived = true;
    }

    this->registerDataFetching(data.isDataReceived);
    
    return data;
}