#pragma once

#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/MCPExtender.h"
#include <BounceMcp.h>

enum class ButtonPressed {NONE, TOUCH, LEFT, CENTER, RIGHT};

class InputsController
{
public:
    InputsController(HardwareRegistry* hardwareRegistry);
    ButtonPressed updateInputs();

private:
    Button* _touchButton;
    Button* _leftButton;
    Button* _centerButton;
    Button* _rightButton;    

    Button* registerButton(MCP* mcp, uint8_t buttonPin, bool isTouch = false);
};