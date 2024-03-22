#include <Arduino.h>
#include "fclib/Component.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::COMPONENT
{
    Led::Led() : OutputPinComponent()
    {
        log.setModuleName("Led");
    }

    Led::~Led()
    {
    }

}