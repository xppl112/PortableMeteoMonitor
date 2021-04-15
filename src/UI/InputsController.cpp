#include "UI/InputsController.h"
#include "Config.h"

InputsController::InputsController(HardwareRegistry* HardwareRegistry){
    _leftButton = registerButton(HardwareRegistry, LEFT_BUTTON_PIN);
    _rightButton = registerButton(HardwareRegistry, RIGHT_BUTTON_PIN);
    _upButton = registerButton(HardwareRegistry, UP_BUTTON_PIN);
    _downButton = registerButton(HardwareRegistry, DOWN_BUTTON_PIN);
}

ButtonPressed InputsController::updateInputs(){
    _leftButton->update();
    _rightButton->update();
    _upButton->update();
    _downButton->update();

    bool leftButtonPressed = _leftButton->pressed();
    bool rightButtonPressed = _rightButton->pressed();   
    bool upButtonPressed = _upButton->pressed(); 
    bool downButtonPressed = _downButton->pressed();    

    if (leftButtonPressed && rightButtonPressed) return ButtonPressed::LEFTRIGHT;
    else if (leftButtonPressed) return ButtonPressed::LEFT;
    else if (rightButtonPressed) return ButtonPressed::RIGHT;
    else if (upButtonPressed) return ButtonPressed::UP;
    else if (downButtonPressed) return ButtonPressed::DOWN;

    return ButtonPressed::NONE;
}

Button* InputsController::registerButton(HardwareRegistry* HardwareRegistry, uint8_t buttonPin){
    Button* button = new Button(HardwareRegistry->mcpExtender); 
    button->attach( buttonPin , INPUT_PULLUP );
    button->interval(5);
    button->setPressedState(LOW);

    return button;
}