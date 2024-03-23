#include <Arduino.h>
#include "fclib/Hardware.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    OutputPinComponent::OutputPinComponent() : PinComponent()
    {
        log.setModuleName("OutputPinComponent");
        this->pin = pin;
        state = false;
        setupPin();
    }

    OutputPinComponent::~OutputPinComponent()
    {
    }

    bool OutputPinComponent::setupPin()
    {
        if (pin < 0)
        {
            return false;
        }

        pinMode(pin, OUTPUT);
        state = false;
        digitalWrite(pin, LOW);
        return true;
    }

    void OutputPinComponent::high()
    {
        if (pin >= 0)
        {
            log.never("set pin %d high", pin);
            digitalWrite(pin, HIGH);
            state = true;
        }
    }

    void OutputPinComponent::low()
    {
        if (pin >= 0)
        {
            log.never("set pin %d low", pin);
            digitalWrite(pin, LOW);
            state = false;
        }
    }

    void OutputPinComponent::setState(bool isHigh)
    {
        if (isHigh)
        {
            high();
        }
        else
        {
            low();
        }
    }
}