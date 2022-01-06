#include "UI/Screens/Elements/MetricTile.h"   
#include <Adafruit_GFX.h> 
    
MetricTile::MetricTile(
    Adafruit_ST7789* screen, 
    uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
    bool displayRefreshTime){
        _screen = screen;
        _x = x;
        _y = y;
        _width = width;
        _height = height;
        _displayRefreshTime = displayRefreshTime;
        _value = 0;
}

void MetricTile::setupGraph(
    bool displayGraph, 
    bool displayScale, 
    TileGraphColor graphColor, 
    uint16_t refreshRateIntervalSeconds){
        _displayGraph = displayGraph;
        _displayScale = displayScale;
        _graphColor = graphColor;
        _refreshRateIntervalSeconds = refreshRateIntervalSeconds;
}

void MetricTile::setValues(std::list<TileDataItem> values){
    _values.clear();
    for (auto const& value : values){
        _values.push_back(value);
        _value = value.value;
    }
}

void MetricTile::redraw(bool fullRedraw){
    _screen->setTextWrap(false);

    _screen->fillRect(_x,_y,_width,_height,BackgroundColor);
    _screen->setTextSize(3);
    _screen->setTextColor(ST77XX_BLACK);
    _screen->setCursor(_x+21, _y+11);_screen->print(_title);
    _screen->setTextColor(ST77XX_WHITE);
    _screen->setCursor(_x+20, _y+10);_screen->print(_title);

    if(_values.size() > 0){
        _screen->setTextSize(4);
        _screen->setTextColor(ST77XX_BLACK);
        _screen->setCursor(_x+21, _y+31);_screen->print(_value);
        _screen->setTextColor(ST77XX_WHITE);
        _screen->setCursor(_x+20, _y+30);_screen->print(_value);
    }
    else {
        _screen->setTextSize(4);
        _screen->setTextColor(ST77XX_BLACK);
        _screen->setCursor(_x+21, _y+31);_screen->print("-/-");
        _screen->setTextColor(ST77XX_WHITE);
        _screen->setCursor(_x+20, _y+30);_screen->print("-/-");
    }
}