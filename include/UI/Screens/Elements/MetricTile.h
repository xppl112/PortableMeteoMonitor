#pragma once

#include <Adafruit_ST7789.h>
#include <vector>

enum class TileStatus { NORMAL, WARNING, ALERT };
enum class TileGraphColor { BLUE, PURPLE, GREEN, YELLOW, RED };

struct TileDataItem
{
public:
    unsigned long timestamp;
    float value;
};

struct GraphDesign
{
public:
    uint16_t LineColor;    
    uint16_t BackgroundColor;
};

class MetricTile
{
public:
    MetricTile(
        Adafruit_ST7789* screen, 
        uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
        bool displayRefreshTime, uint8_t decimalCount = 0);

    void setTitle(String title) {_title = title;}
    void setStatus(TileStatus status);
    void setGraphColor(TileGraphColor graphColor) {_initialGraphColor = _currentGraphColor = graphColor;}

    void setValue(float value) {_currentValue = TileDataItem {.value = value};}
    void setValues(std::vector<TileDataItem> values);

    void setupGraph(bool displayGraph, bool displayScale, TileGraphColor graphColor, bool changeColorByStatus = true);
    
    void redraw(bool fullRedraw = false);
    
private:
    Adafruit_ST7789* _screen;

    String _title;
    TileStatus _status = TileStatus::NORMAL;
    TileGraphColor _initialGraphColor = TileGraphColor::BLUE;
    TileGraphColor _currentGraphColor = TileGraphColor::BLUE;

    bool _displayGraph = false;
    bool _displayScale = false;
    bool _displayRefreshTime = false;
    bool _changeGraphColorByStatus = true;

    TileDataItem _currentValue;
    std::vector<TileDataItem> _values;
    uint8_t _decimalCount;
    
    uint16_t _x, _y;
    uint16_t _width, _height;

    int calculateRefreshRate();

    String stringifyValue(float value);
    String stringifyNumber(float number);
    String stringifyTimeInterval(int seconds);
 
    static const uint16_t BackgroundColor = 0x18E3;
    static const uint16_t ScaleColor = 0x94B2;  

    static const uint16_t WarningBorderColor = 0xF5E7;
    static const uint16_t AlertBorderColor = 0xF800;

    GraphDesign _graphDesign[5] = {
        GraphDesign {.LineColor = 0x4399, .BackgroundColor = 0x3291},//blue
        GraphDesign {.LineColor = 0x8B75, .BackgroundColor = 0x726E},//purple
        GraphDesign {.LineColor = 0x7D4D, .BackgroundColor = 0x43E7},//green
        GraphDesign {.LineColor = 0xF5E7, .BackgroundColor = 0x8367},//yellow
        GraphDesign {.LineColor = 0xD924, .BackgroundColor = 0x90E3} //red
    };
};
