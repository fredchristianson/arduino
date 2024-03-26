#include <Arduino.h>
#include "fclib/Hardware.h"
#include "fclib/System.h"

using namespace FCLIB;

namespace FCLIB::HW
{

    InputPinComponent::InputPinComponent() : PinComponent()
    {
        log.setModuleName("InputPinComponent");
        log.always("----- InputPinComponent 0x%lx", this);
        currentState = 0;
        lastStateChangeMsecs = 0;
        debounceMsecs = DEBOUNCE_MSECS;
        debounceState = 0;
        logTimer.seconds(5);
    }

    InputPinComponent::~InputPinComponent()
    {
    }

    bool InputPinComponent::setupPin()
    {
        log.always("setup pin %d  0x%lx", pin, this);
        if (pin < 0)
        {
            return false;
        }

        log.always("\tinput");
        pinMode(pin, INPUT);
        currentState = digitalRead(pin);
        debounceState = currentState;
        lastStateChangeMsecs = THE_BOARD->currentMsecs();
        Task::repeat([this]()
                     { this->doTask(); })
            ->delayMsecs(5);
        return true;
    }
    int InputPinComponent::getState()
    {
        // return digitalRead(pin);
        return currentState;
    }

    void InputPinComponent::doTask()
    {
        if (pin < 0)
        {
            return;
        }
        int state = digitalRead(pin);
        log.conditional(logTimer.isComplete(), NEVER_LEVEL, "debounce state %d %d %d %d", currentState, state, debounceState, lastStateChangeMsecs);
        if (state == currentState)
        {
            return;
        }
        if (state != debounceState)
        {
            log.never("debounce state change %d %d %d %d", currentState, state, debounceState, lastStateChangeMsecs);
            debounceState = state;
            lastStateChangeMsecs = THE_BOARD->currentMsecs();
        }
        else if (lastStateChangeMsecs + debounceMsecs < THE_BOARD->currentMsecs())
        {
            log.never("debounce time expired %d %d %d %d", currentState, state, debounceState, lastStateChangeMsecs);

            currentState = state;
            log.never("trigger CHANGE_EVENT");
            log.always("trigger CHANGE_EVENT 0x%lx", this);
            Event::trigger(EventType::CHANGE_EVENT, this, currentState != 0);
            if (state == 0)
            { // released
                log.never("trigger PRESS_EVENT");
                Event::trigger(EventType::PRESS_EVENT, this, currentState != 0);
            }
        }
    }
}