#include "UI/Screens/DashboardScreen.h"
#include "StringUtils.h"
#include "config.h"
#include <Adafruit_GFX.h>

DashboardScreen::DashboardScreen(Adafruit_ST7789* screen){
    _screen = screen;
    _co2Tile = new MetricTile(_screen,2,2,157,70,true);
    _co2Tile->setupGraph(true, true, TileGraphColor::BLUE, 30);
    _co2Tile->setTitle("CO2");

    _PMTile = new MetricTile(_screen,2,74,157,70,true);
    _PMTile->setupGraph(true, true, TileGraphColor::BLUE, 30);
    _PMTile->setTitle("PM2.5");

    _ch2oTile = new MetricTile(_screen,2,146,157,70,true);
    _ch2oTile->setupGraph(true, true, TileGraphColor::BLUE, 30);
    _ch2oTile->setTitle("CH2O");

    _outTempTile = new MetricTile(_screen,161,2,157,70,true);
    _outTempTile->setupGraph(true, true, TileGraphColor::PURPLE, 30);
    _outTempTile->setTitle("out t");

    _outPMTile = new MetricTile(_screen,161,74,157,70,true);
    _outPMTile->setupGraph(true, true, TileGraphColor::PURPLE, 30);
    _outPMTile->setTitle("out PM2.5");
}

void DashboardScreen::show(PresentingData data){
    if(data.weatherMonitorHistoricalData.size() != 0){
        std::list<TileDataItem> tileData(DATA_COLLECTION_CAPACITY);
        //CO2
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.CO2
            });
        }
        _co2Tile->setValues(tileData);

        //PM_2_5
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.PM_2_5
            });
        }
        _PMTile->setValues(tileData);

        //CH2O
        tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  (float)dataItem.CH2O
            });
        }
        _ch2oTile->setValues(tileData);

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
        /*tileData.clear();
        for (auto const& dataItem : data.weatherMonitorHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.temperatureCelsium
            });
        }
        _outPMTile->setValues(tileData);*/
    }

    _co2Tile->redraw();
    _PMTile->redraw();
    _ch2oTile->redraw();
    _outTempTile->redraw();
    _outPMTile->redraw();
}
