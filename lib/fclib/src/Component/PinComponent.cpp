#include <Arduino.h>
#include "fclib/Component.h"

using namespace FCLIB;

namespace FCLIB::COMPONENT
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