#pragma once

struct WeatherMonitorData
{
public:
    long timestamp;

    int CO2 = -1;
    int CH2O = -1;

    int PM_1_0 = -1;
    int PM_2_5 = -1;
    int PM_10_0 = -1;

    float temperatureCelsium;
    int humidityPercent;
    float pressureInHPascals;
};