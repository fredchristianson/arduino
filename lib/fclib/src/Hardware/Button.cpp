#include <Arduino.h>
#include "fclib/Hardware.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    Button::Button() : InputPinComponent()
    {
        log.setModuleName("Button");
    }

    Button::~Button()
    {
    }

    bool Button::isPressed()
    {
        return currentState;
    }
}