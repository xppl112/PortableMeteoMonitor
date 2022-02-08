#include "UI/InputsController.h"
#include "Config.h"

InputsController::InputsController(HardwareRegistry* hardwareRegistry){
    auto mcpExtender = hardwareRegistry->_MCPExtender;

    _touchButton = registerButton(mcpExtender->get(), TOUCH_BUTTON_PIN, true);
    _leftButton = registerButton(mcpExtender->get(), LEFT_BUTTON_PIN);
    _centerButton = registerButton(mcpExtender->get(), CENTER_BUTTON_PIN);
    _rightButton = registerButton(mcpExtender->get(), RIGHT_BUTTON_PIN);
}

ButtonPressed InputsController::updateInputs(){
    _touchButton->update();
    _leftButton->update();
    _centerButton->update();
    _rightButton->update();
 
    bool touchButtonPressed = _touchButton->pressed(); 
    bool leftButtonPressed = _leftButton->pressed();
    bool centerButtonPressed = _centerButton->pressed(); 
    bool rightButtonPressed = _rightButton->pressed();     

    if (touchButtonPressed) return ButtonPressed::TOUCH;
    else if (leftButtonPressed) return ButtonPressed::LEFT;
    else if (centerButtonPressed) return ButtonPressed::CENTER;
    else if (rightButtonPressed) return ButtonPressed::RIGHT;

    return ButtonPressed::NONE;
}

Button* InputsController::registerButton(MCP* mcp, uint8_t buttonPin, bool isTouch){
    Button* button = new Button(mcp); 
    button->attach(buttonPin, isTouch ? INPUT : INPUT_PULLUP);
    button->interval(5);
    button->setPressedState(isTouch ? HIGH : LOW);

    return button;
}