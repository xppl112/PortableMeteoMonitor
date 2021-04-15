#ifndef INPUTSCONTROLLER_H
#define INPUTSCONTROLLER_H

#include "HardwareModules/HardwareRegistry.h"
#include <BounceMcp.h>

enum class ButtonPressed {NONE, TOUCH, LEFT, CENTER, RIGHT};

class InputsController
{
public:
    InputsController(HardwareRegistry* HardwareRegistry);
    ButtonPressed updateInputs();

private:
    Button* _leftButton;
    Button* _rightButton;    
    Button* _upButton;
    Button* _downButton;

    Button* registerButton(HardwareRegistry* HardwareRegistry, uint8_t buttonPin);
};

#endif