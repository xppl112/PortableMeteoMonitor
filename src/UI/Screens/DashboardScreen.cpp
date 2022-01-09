#include "UI/Screens/DashboardScreen.h"
#include "StringUtils.h"
#include "config.h"
#include <Adafruit_GFX.h>

DashboardScreen::DashboardScreen(Adafruit_ST7789* screen){
    _screen = screen;
    _co2Tile = new MetricTile(_screen,0,0,158,72,true);
    _co2Tile->setupGraph(true, true, TileGraphColor::BLUE);
    _co2Tile->setTitle("CO2");

    _PMTile = new MetricTile(_screen,0,74,158,72,true);
    _PMTile->setupGraph(true, true, TileGraphColor::BLUE);
    _PMTile->setTitle("PM2.5");

    _ch2oTile = new MetricTile(_screen,0,148,158,72,true);
    _ch2oTile->setupGraph(true, true, TileGraphColor::BLUE);
    _ch2oTile->setTitle("CH2O");

    _outTempTile = new MetricTile(_screen,160,0,158,72,true,1);
    _outTempTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outTempTile->setTitle("out t");

    _outPMTile = new MetricTile(_screen,160,74,158,72,true);
    _outPMTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outPMTile->setTitle("out PM2.5");

    _outPressureTile = new MetricTile(_screen,160,148,158,72,true);
    _outPressureTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outPressureTile->setTitle("out pressure");
}

void DashboardScreen::show(PresentingData data){
    if(data.weatherMonitorHistoricalData.size() != 0){
        std::vector<TileDataItem> tileData(DATA_COLLECTION_CAPACITY);
        //CO2
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            if(dataItem.CO2 == -1)continue;
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.CO2
            });
        }
        _co2Tile->setValues(tileData);
        _co2Tile->setStatus(mapWarningLevelToTileStatus(data.CO2WarningLevel));

        //PM_2_5
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            if(dataItem.PM_2_5 == -1)continue;
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.PM_2_5
            });
        }
        _PMTile->setValues(tileData);
        _PMTile->setStatus(mapWarningLevelToTileStatus(data.PMWarningLevel));

        //CH2O
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            if(dataItem.CH2O == -1)continue;
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.CH2O
            });
        }
        _ch2oTile->setValues(tileData);
        _ch2oTile->setStatus(mapWarningLevelToTileStatus(data.CH2OWarningLevel));

        //temp
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.temperatureCelsium
            });
        }
        _outTempTile->setValues(tileData);

        //out PM2.5
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.humidityPercent
            });
        }
        _outPMTile->setValues(tileData);

        //out Pressure
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.pressureInHPascals
            });
        }
        _outPressureTile->setValues(tileData);
    }

    _co2Tile->redraw();
    _PMTile->redraw();
    _ch2oTile->redraw();
    _outTempTile->redraw();
    _outPMTile->redraw();
    _outPressureTile->redraw();
}

TileStatus DashboardScreen::mapWarningLevelToTileStatus(WarningLevel level){
    if(level == WarningLevel::WARNING)return TileStatus::WARNING;
    if(level == WarningLevel::HI_WARNING_LEVEL)return TileStatus::ALERT;
    return TileStatus::NORMAL;
}