#include <Arduino.h>
#include "fclib/Component.h"
#include "fclib/System.h"

using namespace FCLIB;

namespace FCLIB::COMPONENT
{
    InputPinComponent::InputPinComponent() : PinComponent()
    {
        log.setModuleName("InputPinComponent");
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
        log.always("setup pin %d", pin);
        if (pin < 0)
        {
            return false;
        }

        log.always("\tinput");
        pinMode(pin, INPUT);
        currentState = digitalRead(pin);
        debounceState = currentState;
        lastStateChangeMsecs = THE_BOARD->currentMsecs();
        Task::repeat(*this)->delayMsecs(5);
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
        log.conditional(logTimer.isComplete(), DEBUG_LEVEL, "debounce state %d %d %d %d", currentState, state, debounceState, lastStateChangeMsecs);
        if (state == currentState)
        {
            return;
        }
        if (state != debounceState)
        {
            log.debug("debounce state change %d %d %d %d", currentState, state, debounceState, lastStateChangeMsecs);
            debounceState = state;
            lastStateChangeMsecs = THE_BOARD->currentMsecs();
        }
        else if (lastStateChangeMsecs + debounceMsecs < THE_BOARD->currentMsecs())
        {
            log.debug("debounce time expired %d %d %d %d", currentState, state, debounceState, lastStateChangeMsecs);

            currentState = state;
            log.debug("trigger CHANGE_EVENT");
            Event::trigger(this, CHANGE_EVENT, currentState != 0);
            if (state == 0)
            { // released
                log.debug("trigger PRESS_EVENT");
                Event::trigger(this, PRESS_EVENT, currentState != 0);
            }
        }
    }
}