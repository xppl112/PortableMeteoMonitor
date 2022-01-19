#include "UI/Screens/DashboardScreen.h"
#include "StringUtils.h"
#include "config.h"
#include <Adafruit_GFX.h>

DashboardScreen::DashboardScreen(Adafruit_ST7789* screen){
    _screen = screen;
    _co2Tile = new MetricTile(_screen,0,0,159,72,true);
    _co2Tile->setupGraph(true, true, TileGraphColor::BLUE);
    _co2Tile->setTitle("CO2");

    _PMTile = new MetricTile(_screen,0,74,159,72,true);
    _PMTile->setupGraph(true, true, TileGraphColor::BLUE);
    _PMTile->setTitle("PM2.5");

    _ch2oTile = new MetricTile(_screen,0,148,159,72,true);
    _ch2oTile->setupGraph(true, true, TileGraphColor::BLUE);
    _ch2oTile->setTitle("CH2O");

    _outTempTile = new MetricTile(_screen,161,0,159,72,true,1);
    _outTempTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outTempTile->setTitle("out t");

    _outPMTile = new MetricTile(_screen,161,74,159,72,true);
    _outPMTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outPMTile->setTitle("out PM2.5");

    _outPressureTile = new MetricTile(_screen,161,148,159,72,true);
    _outPressureTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outPressureTile->setTitle("out pressure");
}

void DashboardScreen::showWeatherData(PresentingWeatherData weatherData){
    std::vector<TileDataItem> tileData(DATA_COLLECTION_CAPACITY);

    if(weatherData.weatherMonitorHistoricalData.size() != 0){
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
    }

    _co2Tile->redraw();
    _PMTile->redraw();
    _ch2oTile->redraw();
}

void DashboardScreen::showBackendWeatherData(PresentingBackendWeatherData backendWeatherData){
    std::vector<TileDataItem> tileData(DATA_COLLECTION_CAPACITY);

    if(backendWeatherData.backendWeatherHistoricalData.size() != 0){
        //temp
        tileData.clear();
        for (auto const& dataItem : backendWeatherData.backendWeatherHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.temperatureCelsium
            });
        }
        _outTempTile->setValues(tileData);

        //out PM2.5
        tileData.clear();
        for (auto const& dataItem : backendWeatherData.backendWeatherHistoricalData){
            if(dataItem.PM_2_5 == -1)continue;
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.PM_2_5
            });
        }
        _outPMTile->setValues(tileData);
        _outPMTile->setStatus(mapWarningLevelToTileStatus(backendWeatherData.PMWarningLevel));

        //out Pressure
        tileData.clear();
        for (auto const& dataItem : backendWeatherData.backendWeatherHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.pressureInHPascals
            });
        }
        _outPressureTile->setValues(tileData);
    }

    _outTempTile->redraw();
    _outPMTile->redraw();
    _outPressureTile->redraw();
}

TileStatus DashboardScreen::mapWarningLevelToTileStatus(WarningLevel level){
    if(level == WarningLevel::WARNING)return TileStatus::WARNING;
    if(level == WarningLevel::HI_WARNING_LEVEL)return TileStatus::ALERT;
    return TileStatus::NORMAL;
}