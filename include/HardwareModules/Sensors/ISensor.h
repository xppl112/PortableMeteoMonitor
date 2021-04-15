#ifndef ISensor_H
#define ISensor_H

#include "HardwareModules/IHardwareModule.h"

struct SensorDataBase {
  bool isDataReceived;
};

class ISensor : public IHardwareModule
{
    protected:
        virtual short int getFailedDataFetchingTreshold() = 0;
        void registerDataFetching(bool success){
            // if we don't receive data for times, mark sensor as unconnected
            auto failedDataFetchingTredhold = getFailedDataFetchingTreshold();
            if(!success){
                _failedDataFetchingCount++;
                if(failedDataFetchingTredhold != -1 && 
                   _failedDataFetchingCount >= failedDataFetchingTredhold)
                    this->_isConnected = false;
            }
            else _failedDataFetchingCount = 0;

        }
        int _failedDataFetchingCount = 0;
};

#endif