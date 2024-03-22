#include <Arduino.h>
#include "fclib/Component.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::COMPONENT
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