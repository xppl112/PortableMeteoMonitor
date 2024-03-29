#include "UI/Screens/LocalDataScreen.h"
#include "StringUtils.h"
#include "config.h"
#include <Adafruit_GFX.h>

LocalDataScreen::LocalDataScreen(Adafruit_ST7789* screen){
    _screen = screen;
    _co2Tile = new MetricTile(_screen,0,0,159,72,false);
    _co2Tile->setupGraph(true, true, TileGraphColor::BLUE);
    _co2Tile->setTitle("CO2");

    _PMTile = new MetricTile(_screen,0,74,159,72,false);
    _PMTile->setupGraph(true, true, TileGraphColor::BLUE);
    _PMTile->setTitle("PM2.5");

    _ch2oTile = new MetricTile(_screen,0,148,159,72,true);
    _ch2oTile->setupGraph(true, true, TileGraphColor::BLUE);
    _ch2oTile->setTitle("CH2O");

    _tempTile = new MetricTile(_screen,161,0,159,72,false,1);
    _tempTile->setupGraph(true, true, TileGraphColor::BLUE);
    _tempTile->setTitle("temperature");

    _humTile = new MetricTile(_screen,161,74,159,72,false);
    _humTile->setupGraph(true, true, TileGraphColor::BLUE);
    _humTile->setTitle("humidity");

    _pressureTile = new MetricTile(_screen,161,148,159,72,true);
    _pressureTile->setupGraph(true, false, TileGraphColor::BLUE);
    _pressureTile->setTitle("pressure");
}

void LocalDataScreen::showWeatherData(PresentingWeatherData weatherData){
    std::vector<TileDataItem> tileData(DATA_COLLECTION_CAPACITY);

    //CO2
    tileData.clear();
    for (auto const& dataItem : weatherData.weatherMonitorHistoricalData){
        if(dataItem.CO2 == -1)continue;
        tileData.push_back(TileDataItem {
            .timestamp =  dataItem.timestamp,
            .value =  (float)dataItem.CO2
        });
    }
    _co2Tile->setValues(tileData);
    _co2Tile->setStatus(mapWarningLevelToTileStatus(weatherData.CO2WarningLevel));

    //PM_2_5
    tileData.clear();
    for (auto const& dataItem : weatherData.weatherMonitorHistoricalData){
        if(dataItem.PM_2_5 == -1)continue;
        tileData.push_back(TileDataItem {
            .timestamp =  dataItem.timestamp,
            .value =  (float)dataItem.PM_2_5
        });
    }
    _PMTile->setValues(tileData);
    _PMTile->setStatus(mapWarningLevelToTileStatus(weatherData.PMWarningLevel));

    //CH2O
    tileData.clear();
    for (auto const& dataItem : weatherData.weatherMonitorHistoricalData){
        if(dataItem.CH2O == -1)continue;
        tileData.push_back(TileDataItem {
            .timestamp =  dataItem.timestamp,
            .value =  (float)dataItem.CH2O
        });
    }
    _ch2oTile->setValues(tileData);
    _ch2oTile->setStatus(mapWarningLevelToTileStatus(weatherData.CH2OWarningLevel));

    //temp
    tileData.clear();
    for (auto const& dataItem : weatherData.weatherMonitorHistoricalData){
        tileData.push_back(TileDataItem {
            .timestamp =  dataItem.timestamp,
            .value =  dataItem.temperatureCelsium
        });
    }
    _tempTile->setValues(tileData);

    //humidity
    tileData.clear();
    for (auto const& dataItem : weatherData.weatherMonitorHistoricalData){
        tileData.push_back(TileDataItem {
            .timestamp =  dataItem.timestamp,
            .value =  (float)dataItem.humidityPercent
        });
    }
    _humTile->setValues(tileData);

    //pressure
    tileData.clear();
    for (auto const& dataItem : weatherData.weatherMonitorHistoricalData){
        tileData.push_back(TileDataItem {
            .timestamp =  dataItem.timestamp,
            .value =  dataItem.pressureInHPascals
        });
    }
    _pressureTile->setValues(tileData);


    _co2Tile->redraw();
    _PMTile->redraw();
    _ch2oTile->redraw();
    _tempTile->redraw();
    _humTile->redraw();
    _pressureTile->redraw();
}

TileStatus LocalDataScreen::mapWarningLevelToTileStatus(WarningLevel level){
    if(level == WarningLevel::WARNING)return TileStatus::WARNING;
    if(level == WarningLevel::HI_WARNING_LEVEL)return TileStatus::ALERT;
    return TileStatus::NORMAL;
}