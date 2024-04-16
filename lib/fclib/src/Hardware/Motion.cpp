#include <Arduino.h>
#include "fclib/Hardware.h"
#include "fclib/Timer.h"

using namespace FCLIB;

namespace FCLIB::HW
{
    Motion::Motion() : InputPinComponent()
    {
        log.setModuleName("HW::Motion");
        log.debug("Motion 0x%lx", this);
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

    MultiMotion::MultiMotion() : log("MultiMotion")
    {
    }

    MultiMotion::~MultiMotion()
    {
    }

    void MultiMotion::addPin(int8 pin)
    {
        if (pin < 0)
        {
            log.error("Invalid pin %d", pin);
            return;
        }
        Motion *m = new Motion();
        m->setPin(pin);
        motion.add(m);
        listener.handleChange(m, [this](Event *event)
                              { Event::trigger(event->getType(), this, event->getData()); });
    }

    bool MultiMotion::isDetected()
    {
        return motion.any([this](Motion *motion)
                          { this->log.debug("check motion pin %d %d", motion->getPin(), motion->isDetected()); 
                          return motion->isDetected(); });
    }
}