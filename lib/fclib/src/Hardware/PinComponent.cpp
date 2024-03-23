#include <Arduino.h>
#include "fclib/Hardware.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    PinComponent::PinComponent()
    {
        log.setModuleName("PinComponent");
        this->pin = pin;
    }

    PinComponent::~PinComponent()
    {
    }

    void PinComponent::setPin(int8 pin)
    {
        this->pin = pin;
        this->setupPin();
    }
}