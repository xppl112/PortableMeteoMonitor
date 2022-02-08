#pragma once

struct BackendWeatherData
{
public:
    long timestamp;

    int PM_2_5 = -1;

    float temperatureCelsium;
    float humidityPercent;
    float pressureInHPascals;

    float temperatureBalconyCelsium;
    float humidityBalconyPercent;
};