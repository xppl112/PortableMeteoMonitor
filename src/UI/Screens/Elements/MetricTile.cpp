#include "UI/Screens/Elements/MetricTile.h"   
#include "UI/Screens/CommonDrawFunctions.h"   
#include <Adafruit_GFX.h> 
#include "Fonts/FreeSans9pt7b.h" 
#include "Fonts/FreeSansBold18pt7b.h" 
#include "config.h" 
    
MetricTile::MetricTile(
    Adafruit_ST7789* screen, 
    uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
    bool displayRefreshTime, uint8_t decimalCount){
        _screen = screen;
        _x = x;
        _y = y;
        _width = width;
        _height = height;
        _displayRefreshTime = displayRefreshTime;
        _decimalCount = decimalCount;
}

void MetricTile::setupGraph(
    bool displayGraph, 
    bool displayScale, 
    TileGraphColor graphColor, 
    bool changeColorByStatus){
        _displayGraph = displayGraph;
        _displayScale = displayScale;
        _initialGraphColor = _currentGraphColor = graphColor;
        _changeGraphColorByStatus = changeColorByStatus;
}

void MetricTile::setValues(std::vector<TileDataItem> values){
    _values.clear();
    for (auto const& value : values){
        _values.push_back(value);
        _currentValue = value;
    }
}

void MetricTile::redraw(bool fullRedraw){
    _screen->setTextWrap(false);

    _screen->fillRect(_x,_y,_width,_height,BackgroundColor);

    //Graph
    if(_displayGraph && _values.size() > 0){
        float minValue = 99999, maxValue = -99999;
        for (auto const& value : _values){
            if(value.value > maxValue)maxValue = value.value;
            if(value.value < minValue)minValue = value.value;
        }

        auto refreshRateSeconds = calculateRefreshRate();
        float graphStepWidth = (float)_width / (float)DATA_COLLECTION_CAPACITY;
        uint8_t graphXPositionOffset = (millis() - _values.back().timestamp) / 1000 * graphStepWidth / refreshRateSeconds;
        if(graphXPositionOffset > graphStepWidth)graphXPositionOffset = graphStepWidth;
        float graphXPosition = (float)_width + (float)_x - (float)graphXPositionOffset;

        uint16_t previousGraphStepX = graphXPosition;
        for(int i = _values.size() - 1; i >= 0; i--){
            uint16_t graphStepX = graphXPosition - graphStepWidth;
            if(graphStepX < _x)graphStepX = _x;

            //TODO: add dynamic calculation. For now: 15 - min, 63 - max
            uint16_t graphStepMinHeight = 15;
            uint16_t graphStepMaxHeight = 63;
            uint16_t graphStepHeight = graphStepMinHeight + 16;
            if(maxValue != minValue)
                graphStepHeight = graphStepMinHeight + 
                    (int)((_values[i].value - minValue) * 
                    ((float)(graphStepMaxHeight - graphStepMinHeight) / (maxValue - minValue)) + 0.5);

            _screen->fillRect(
                graphStepX, _y + _height - graphStepHeight + 2, 
                previousGraphStepX - graphStepX, graphStepHeight - 2, _graphDesign[(int)_currentGraphColor].BackgroundColor);
            _screen->fillRect(
                graphStepX, _y + _height - graphStepHeight, 
                previousGraphStepX - graphStepX, 2, _graphDesign[(int)_currentGraphColor].LineColor);

            graphXPosition -= graphStepWidth;
            previousGraphStepX = graphStepX;
        }

        //Scale
        if(_displayScale){
            float scaleStage = (maxValue - minValue) / 3;

            _screen->setFont(NULL);_screen->setTextSize(1);
            _screen->setTextColor(ScaleColor);

            String scaleStageText = stringifyNumber(maxValue);
            _screen->setCursor(_x + _width - 4 - CommonDrawFunctions::getTextWidth(_screen, scaleStageText),_y + 6);
            _screen->print(scaleStageText);
            _screen->drawLine(_x + _width - 4, _y + 9, _x + _width -1, _y + 9, ScaleColor);

            scaleStageText = stringifyNumber(minValue + scaleStage * 2);
            _screen->setCursor(_x + _width - 4 - CommonDrawFunctions::getTextWidth(_screen, scaleStageText),_y + 22);
            _screen->print(scaleStageText);
            _screen->drawLine(_x + _width - 4, _y + 25, _x + _width -1, _y + 25, ScaleColor);

            scaleStageText = stringifyNumber(minValue + scaleStage);
            _screen->setCursor(_x + _width - 4 - CommonDrawFunctions::getTextWidth(_screen, scaleStageText),_y + 38);
            _screen->print(scaleStageText);
            _screen->drawLine(_x + _width - 4, _y + 41, _x + _width -1, _y + 41, ScaleColor);

            scaleStageText = stringifyNumber(minValue);
            _screen->setCursor(_x + _width - 4 - CommonDrawFunctions::getTextWidth(_screen, scaleStageText),_y + 54);
            _screen->print(scaleStageText);
            _screen->drawLine(_x + _width - 4, _y + 57, _x + _width -1, _y + 57, ScaleColor);
        }
    }
    //Refresh time
    if(_displayRefreshTime && _values.size() > 0){
        int lastRefreshSeconds = (millis() - _currentValue.timestamp) / 1000;

        _screen->setFont(NULL);_screen->setTextSize(1);
        _screen->setTextColor(ScaleColor);
        _screen->setCursor(_x + 5,_y + _height - 12);
        _screen->print(stringifyTimeInterval(lastRefreshSeconds));
    }

    //Title & Value
    CommonDrawFunctions::drawCenteredText(_screen, _title, 9, ST77XX_WHITE, _x, _x+_width - 10,_y + 20);

    if(_values.size() > 0){
        CommonDrawFunctions::drawCenteredText(_screen, stringifyValue(_currentValue.value), 18, ST77XX_WHITE, _x, _x+_width - 10,_y + 53);
    }
    else {
        CommonDrawFunctions::drawCenteredText(_screen, "-/-", 18, ST77XX_WHITE, _x, _x+_width - 10,_y + 53);
    }

    //Border
    if(_status != TileStatus::NORMAL){
        _screen->drawRect(
            _x,_y,_width,_height, _status == TileStatus::WARNING ? WarningBorderColor : AlertBorderColor);
    }
}

void MetricTile::setStatus(TileStatus status) {
    _status = status;
    if(_changeGraphColorByStatus){
        if(status == TileStatus::WARNING)_currentGraphColor = TileGraphColor::YELLOW;
        else if(status == TileStatus::ALERT)_currentGraphColor = TileGraphColor::RED;
        else _currentGraphColor = _initialGraphColor;
    }
}

int MetricTile::calculateRefreshRate(){
    if(_values.size() < 2) return 300;
    int refreshRates[_values.size() - 1];
    for(uint8_t i = _values.size() - 1; i > 0; i--){
        refreshRates[i - 1] = _values[i].timestamp - _values[i-1].timestamp;
    }
    //choose the actual refresh rate. For now, just average
    int refreshRateSeconds = 0;
    for(uint8_t i = 0; i<_values.size() - 1;i++)refreshRateSeconds += refreshRates[i];
    return refreshRateSeconds / (_values.size() - 1) / 1000;
}

String MetricTile::stringifyValue(float value){
    return String(value, _decimalCount);
}

String MetricTile::stringifyNumber(float number){
    if((number > 100 && number < 1000) || (number < -100 && number > -1000)) 
        number = (int)(number / 10) * 10;

    if(number < 1000 && number > -1000)return String((int)number);
    if(number < 10000 && number > -10000)return String(number/(float)1000,1) + "k";
    if(number < 100000 && number > -100000)return String((int)(number/1000)) + "k";
    if(number < 1000000 && number > -1000000)return String(number/(float)1000000,1) + "m";
    return String((int)(number/1000000)) + "m";
}

String MetricTile::stringifyTimeInterval(int seconds){
    if(seconds < 60 * 3)return "<" + String((int)(seconds/60)+1) + "m";
    if(seconds < 60 * 60)return "~" + String((int)(seconds/60)) + "m";
    if(seconds < 60 * 60 * 24)return "~" + String((int)(seconds/60/60)) + "h";
    return "~" + String((int)(seconds/60/60/24)) + "d";
}