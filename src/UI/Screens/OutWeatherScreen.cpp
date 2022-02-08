#include "UI/Screens/OutWeatherScreen.h"
#include "StringUtils.h"
#include "config.h"
#include <Adafruit_GFX.h>

OutWeatherScreen::OutWeatherScreen(Adafruit_ST7789* screen){
    _screen = screen;
    _outTempTile = new MetricTile(_screen,0,0,159,72,false,1);
    _outTempTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outTempTile->setTitle("out t");

    _outHumTile = new MetricTile(_screen,0,74,159,72,false);
    _outHumTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outHumTile->setTitle("out humidity");

    _outPMTile = new MetricTile(_screen,0,148,159,72,true);
    _outPMTile->setupGraph(true, true, TileGraphColor::PURPLE);
    _outPMTile->setTitle("out PM2.5");

    _balconyTempTile = new MetricTile(_screen,161,0,159,72,false,1);
    _balconyTempTile->setupGraph(true, true, TileGraphColor::BLUE);
    _balconyTempTile->setTitle("balcony t");

    _balconyHumTile = new MetricTile(_screen,161,74,159,72,false);
    _balconyHumTile->setupGraph(true, true, TileGraphColor::BLUE);
    _balconyHumTile->setTitle("balcony hum.");

    _outPressureTile = new MetricTile(_screen,161,148,159,72,true);
    _outPressureTile->setupGraph(true, false, TileGraphColor::PURPLE);
    _outPressureTile->setTitle("out pressure");
}

void OutWeatherScreen::showBackendWeatherData(PresentingBackendWeatherData backendWeatherData){
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

        //temp
        tileData.clear();
        for (auto const& dataItem : backendWeatherData.backendWeatherHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.humidityPercent
            });
        }
        _outHumTile->setValues(tileData);

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

        //balcony Temperature
        tileData.clear();
        for (auto const& dataItem : backendWeatherData.backendWeatherHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.temperatureBalconyCelsium
            });
        }
        _balconyTempTile->setValues(tileData);

        //balcony Humidity
        tileData.clear();
        for (auto const& dataItem : backendWeatherData.backendWeatherHistoricalData){
            tileData.push_back(TileDataItem {
                .timestamp =  dataItem.timestamp,
                .value =  dataItem.humidityBalconyPercent
            });
        }
        _balconyHumTile->setValues(tileData);
    }

    _outTempTile->redraw();
    _outHumTile->redraw();
    _outPMTile->redraw();
    _balconyTempTile->redraw();
    _balconyHumTile->redraw();
    _outPressureTile->redraw();
}

TileStatus OutWeatherScreen::mapWarningLevelToTileStatus(WarningLevel level){
    if(level == WarningLevel::WARNING)return TileStatus::WARNING;
    if(level == WarningLevel::HI_WARNING_LEVEL)return TileStatus::ALERT;
    return TileStatus::NORMAL;
}