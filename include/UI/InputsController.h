#ifndef INPUTSCONTROLLER_H
#define INPUTSCONTROLLER_H

#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/MCPExtender.h"
#include <BounceMcp.h>
#include "Log4Esp.h"

enum class ButtonPressed {NONE, TOUCH, LEFT, CENTER, RIGHT};

class InputsController
{
public:
    InputsController(HardwareRegistry* hardwareRegistry, Logger* logger);
    ButtonPressed updateInputs();

private:
    Logger* _logger;

    Button* _touchButton;
    Button* _leftButton;
    Button* _centerButton;
    Button* _rightButton;    

    Button* registerButton(MCP* mcp, uint8_t buttonPin, bool isTouch = false);
};

#endif