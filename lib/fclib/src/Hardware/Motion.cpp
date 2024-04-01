#include <Arduino.h>
#include "fclib/Hardware.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    Motion::Motion() : InputPinComponent()
    {
        log.setModuleName("HW::Motion");
        log.always("Motion 0x%lx", this);
        debounceMsecs = 0;
    }

    Motion::~Motion()
    {
    }

    bool Motion::isDetected() { return getState(); }

    void Motion::onHigh()
    {
        log.always("got motion");
        Event::trigger(EventType::MOTION_START_EVENT, this, true);
    }
    void Motion::onLow()
    {
        log.always("no motion");

        Event::trigger(EventType::MOTION_STOP_EVENT, this, true);
    }
}