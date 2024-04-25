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

    bool Motion::isDetected() const { return getState() != 0; }

    void Motion::onHigh()
    {
        log.never("got motion");
        Event::trigger(EventType::MOTION_START_EVENT, this, true);
    }
    void Motion::onLow()
    {
        log.never("no motion");

        Event::trigger(EventType::MOTION_STOP_EVENT, this, true);
    }

    MultiMotion::MultiMotion() : log("MultiMotion")
    {
        hasMotion = false;
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
        removePin(pin);
        Motion *m = new Motion();
        m->setPin(pin);
        motion.add(m);
        listener.handleChange(m, [this](Event *event)
                              { this->updateState(); });
    }

    void MultiMotion::updateState()
    {
        bool now = isDetected();
        if (now != hasMotion)
        {
            hasMotion = now;
            log.never("motion changed %s", hasMotion ? "detected" : " clear");
            Event::trigger(now ? EventType::MOTION_START_EVENT : EventType::MOTION_STOP_EVENT, this, now);
            Event::trigger(EventType::CHANGE_EVENT, this, now);
        }
    }
    void MultiMotion::removeAll()
    {
        motion.deleteAll();
    }
    void MultiMotion::removePin(int8 pin)
    {
        if (pin < 0)
        {
            log.error("Invalid pin %d", pin);
            return;
        }
        motion.removeIf([pin, this](const Motion *m)
                        { return m->getPin() == pin; },
                        [pin, this](Motion *m)
                        { this->listener.removeSender(m);
                        delete m; });
    }

    bool MultiMotion::isDetected() const
    {
        return motion.any([this](const Motion *motion)
                          { return motion->isDetected(); });
    }
}