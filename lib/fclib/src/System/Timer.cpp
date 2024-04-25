#include <Arduino.h>
#include <Time.h>
#include "fclib/System.h"
#include "fclib/Timer.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{

    long Timer::toMsecs(size_t duration, TimerUnit unit)
    {
        size_t msecs = 0;
        switch (unit)
        {
        case TIME_MSECS:
            msecs = duration;
            break;
        case TIME_SECONDS:
            msecs = duration * 1000;
            break;
        case TIME_MINUTES:
            msecs = duration * 1000 * 60;
            break;
        default:
            Logger log("Timer");
            log.error("Unknown TimerUnit %d", unit);
        }
        return msecs;
    }

    Timer::Timer(size_t duration, TimerUnit unit) : log("Timer")
    {
        log.debug("Timer::Timer");

        this->durationMsecs = Timer::toMsecs(duration, unit);

        this->startTimeMsecs = this->currentTimeMsecs();
    }

    Timer::~Timer()
    {
    }

    bool Timer::isComplete()
    {
        bool expired = this->startTimeMsecs + this->durationMsecs < this->currentTimeMsecs();
        log.debug("expire test %ld %ld %ld", this->startTimeMsecs, this->durationMsecs, this->currentTimeMsecs());
        return expired;
    }

    bool Timer::reset()
    {
        bool expired = this->startTimeMsecs + this->durationMsecs > this->currentTimeMsecs();

        this->startTimeMsecs = this->currentTimeMsecs();
        return expired;
    }

    bool Timer::reset(size_t duration, TimerUnit unit)
    {
        bool expired = this->startTimeMsecs + this->durationMsecs > this->currentTimeMsecs();

        this->startTimeMsecs = this->currentTimeMsecs();
        this->durationMsecs = Timer::toMsecs(duration, unit);
        return expired;
    }
    unsigned long Timer::currentTimeMsecs()
    {
        return millis();
        // return THE_BOARD->currentMsecs();
    }

    IntervalTimer::IntervalTimer(size_t duration, TimerUnit unit) : Timer(duration, unit)
    {
        log.setModuleName("IntervalTimer");
        log.debug("timer: %d %d", duration, unit);
    }

    IntervalTimer::~IntervalTimer()
    {
    }

    Timer &Timer::seconds(int addSeconds)
    {
        this->durationMsecs += addSeconds * 1000;
        this->startTimeMsecs = this->currentTimeMsecs();
        return *this;
    }
    Timer &Timer::msecs(int addMsecs)
    {
        this->durationMsecs += addMsecs;
        this->startTimeMsecs = this->currentTimeMsecs();
        return *this;
    }
    Timer &Timer::minutes(int addMinutess)
    {
        this->durationMsecs += addMinutess * 60 * 1000;
        this->startTimeMsecs = this->currentTimeMsecs();
        return *this;
    }

    bool IntervalTimer::isComplete()
    {
        long over = this->startTimeMsecs + this->durationMsecs;
        long now = this->currentTimeMsecs();
        log.debug("isComplete %ld %ld %ld %ld", over, now, this->startTimeMsecs, this->durationMsecs);
        Timer::isComplete();
        if (now > over)
        {
            // excess can be (now-over) to "catch up" if last interval took too long.  seems better not to
            long excess = 0; // now - over;
            this->startTimeMsecs = now - excess;
            log.debug("complete");
            return true;
        }
        return false;
    }

    IntervalTimer *IntervalTimer::create(size_t tics, size_t duration, TimerUnit unit)
    {
        Logger log("IntervalTimer");
        log.debug("create IntervalTimer %ld %ld %d", tics, duration, unit);
        return new IntervalTimer(Timer::toMsecs(duration, unit) / tics);
    }

    InstantTimer::InstantTimer() : Timer(0, TIME_MSECS)
    {
        log.setModuleName("InstantTimer");
    }
    bool InstantTimer::isComplete()
    {
        log.debug("complete");
        return true;
    }
} // namespace FCLIB
