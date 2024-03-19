#include <Arduino.h>
#include <Time.h>
#include "fclib/System.h"
#include "fclib/Timer.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{

    Timer::Timer(size_t durationMsecs) : log("Timer")
    {
        log.debug("Timer::Timer");
        this->durationMsecs = durationMsecs;
        this->startTimeMsecs = this->currentTimeMsecs();
    }

    Timer::~Timer()
    {
    }

    bool Timer::isComplete()
    {
        bool expires = this->startTimeMsecs + this->durationMsecs > this->currentTimeMsecs();
        log.debug("expire test %ld %ld %ld", this->startTimeMsecs, this->durationMsecs, this->currentTimeMsecs());
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
        log.setModuleName("IntervalTimer");
        log.debug("timer msecs: %d", msecs);
    }

    IntervalTimer::~IntervalTimer()
    {
    }

    bool IntervalTimer::isComplete()
    {
        long over = this->startTimeMsecs + this->durationMsecs;
        long now = this->currentTimeMsecs();
        log.debug("isComplete %ld %ld %ld %ld", over, now, this->startTimeMsecs, this->durationMsecs);
        Timer::isComplete();
        if (now > over)
        {
            long excess = now - over;
            this->startTimeMsecs = now - excess;
            log.debug("complete");
            return true;
        }
        return false;
    }

    IntervalTimer *IntervalTimer::create(size_t tics, size_t durationMsecs)
    {
        Logger log("IntervalTimer");
        log.debug("create IntervalTimer %ld %ld", tics, durationMsecs);
        return new IntervalTimer(durationMsecs / tics);
    }

    InstantTimer::InstantTimer() : Timer(0)
    {
        log.setModuleName("InstantTimer");
    }
    bool InstantTimer::isComplete()
    {
        log.debug("complete");
        return true;
    }
} // namespace FCLIB
