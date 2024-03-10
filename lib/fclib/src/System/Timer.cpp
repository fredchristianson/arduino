#include <Arduino.h>
#include <Time.h>
#include "fclib/System.h"
#include "fclib/Timer.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{

    Timer::Timer(size_t durationMsecs) : LOG("Timer")
    {
        LOG.debug("Timer::Timer");
        this->durationMsecs = durationMsecs;
        this->startTimeMsecs = this->currentTimeMsecs();
    }

    Timer::~Timer()
    {
    }

    bool Timer::isComplete()
    {
        bool expires = this->startTimeMsecs + this->durationMsecs > this->currentTimeMsecs();
        LOG.debug("expire test %ld %ld %ld", this->startTimeMsecs, this->durationMsecs, this->currentTimeMsecs());
        return expires;
    }

    bool Timer::reset()
    {
        bool expires = this->startTimeMsecs + this->durationMsecs > this->currentTimeMsecs();

        this->startTimeMsecs = this->currentTimeMsecs();
        return expires;
    }

    unsigned long Timer::currentTimeMsecs()
    {
        return millis();
        // return THE_BOARD->currentMsecs();
    }

    IntervalTimer::IntervalTimer(size_t msecs) : Timer(msecs)
    {
        LOG.setModuleName("IntervalTimer");
    }

    IntervalTimer::~IntervalTimer()
    {
    }

    bool IntervalTimer::isComplete()
    {
        long over = this->startTimeMsecs + this->durationMsecs;
        long now = this->currentTimeMsecs();
        LOG.never("isComplete %ld %ld", over, now);
        Timer::isComplete();
        if (now > over)
        {
            long excess = now - over;
            this->startTimeMsecs = now - excess;
            LOG.debug("complete");
            return true;
        }
        return false;
    }

    IntervalTimer *IntervalTimer::create(size_t tics, size_t durationMsecs)
    {
        Logger log("IntervalTimer::create", WARN_LEVEL);
        log.debug("create IntervalTimer %ld %ld", tics, durationMsecs);
        return new IntervalTimer(durationMsecs / tics);
    }

} // namespace FCLIB
