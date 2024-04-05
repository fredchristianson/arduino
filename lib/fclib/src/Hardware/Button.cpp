#include <Arduino.h>
#include "fclib/Hardware.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    Button::Button() : InputPinComponent()
    {
        log.setModuleName("HW::Button");
        log.debug("Button 0x%lx", this);
    }

    Button::~Button()
    {
    }

    bool Button::isPressed()
    {
        return currentState;
    }
}