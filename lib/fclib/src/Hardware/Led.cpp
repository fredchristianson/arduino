#include <Arduino.h>
#include "fclib/Hardware.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    Led::Led() : OutputPinComponent()
    {
        log.setModuleName("Led");
    }

    Led::~Led()
    {
    }

}