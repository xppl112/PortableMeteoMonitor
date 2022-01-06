#pragma once

#include <Adafruit_ST7789.h>
#include <list>

enum class TileStatus { NORMAL, WARNING, ALERT };
enum class TileGraphColor { BLUE, PURPLE, GREEN, YELLOW, RED };

struct TileDataItem
{
public:
    unsigned long timestamp;
    float value;
};

class MetricTile
{
public:
    MetricTile(
        Adafruit_ST7789* screen, 
        uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
        bool displayRefreshTime);

    void setTitle(String title) {_title = title;}
    void setStatus(TileStatus status) {_status = status;}
    void setGraphColor(TileGraphColor graphColor) {_graphColor = graphColor;}

    void setValue(float value) {_value = value;}
    void setValues(std::list<TileDataItem> values);

    void setupGraph(bool displayGraph, bool displayScale, TileGraphColor graphColor, uint16_t refreshRateIntervalSeconds);
    
    void redraw(bool fullRedraw = false);
    
private:
    Adafruit_ST7789* _screen;

    String _title;
    TileStatus _status = TileStatus::NORMAL;
    TileGraphColor _graphColor = TileGraphColor::BLUE;

    bool _displayGraph = false;
    bool _displayScale = false;
    bool _displayRefreshTime = false;
    uint16_t _refreshRateIntervalSeconds = 30;

    float _value;
    std::list<TileDataItem> _values;

    uint16_t _x, _y;
    uint16_t _width, _height;

    static const uint16_t BackgroundColor = 0x18E3;
};
