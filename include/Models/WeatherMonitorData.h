#pragma once

class WeatherMonitorData
{
public:
    int CO2;
    int CH2O;

    int PM_1_0;
    int PM_2_5;
    int PM_10_0;

    float temperatureCelsium;
    float humidityPercent;
    float pressureInHPascals;
};