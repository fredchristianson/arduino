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
        requireAll = false;
    }

    MultiMotion::~MultiMotion()
    {
    }

    void MultiMotion::any() { requireAll = false; }
    void MultiMotion::all() { requireAll = true; }

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
        log.always("Motion: %s (%s)", now ? "detected" : "clear", requireAll ? "all" : "any");
        LogIndent indent;
        for (int i = 0; i < motion.size(); i++)
        {
            log.always("%d: %s", motion[i]->getPin(), motion[i]->isDetected() ? "detected" : "clear");
        }
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
        if (requireAll)
        {
            return motion.all([this](const Motion *motion)
                              { return motion->isDetected(); });
        }
        else
        {
            return motion.any([this](const Motion *motion)
                              { return motion->isDetected(); });
        }
    }
}