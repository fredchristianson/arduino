

#ifndef __FCLIBTIMER_H__
#define __FCLIBTIMER_H__
#include <cstddef>
#include "./Logging.h"
namespace FCLIB
{
    enum TimerUnit
    {
        TIME_MSECS = 0,
        TIME_SECONDS = 1,
        TIME_MINUTES = 2
    };

    class Timer
    {
    public:
        static long toMsecs(size_t duration, TimerUnit unit);
        static Timer *createByFrequency(size_t duration, TimerUnit unit = TIME_MSECS) { return create(1, duration, unit); }

        static Timer *createByRate(float ticsPerSecond) { return create(1, 1000.0 / ticsPerSecond, TIME_MSECS); }

        static Timer *create(size_t tics, size_t duration, TimerUnit unit);

    public:
        Timer(size_t duration, TimerUnit unit);
        virtual ~Timer();
        virtual bool isComplete();
        virtual bool reset();
        virtual void destroy() { delete this; }
        Timer &seconds(int addSeconds);
        Timer &msecs(int addMsecs);
        Timer &minutes(int addMinutess);

        unsigned long getStartTimeMsecs() { return this->startTimeMsecs; }
        unsigned long getDurationMsecs() { return this->durationMsecs; }
        unsigned long currentTimeMsecs();

    protected:
        unsigned long startTimeMsecs;
        unsigned long durationMsecs;
        Logger log;
    };

    /* InstantTime is complete right away*/
    class InstantTimer : public Timer
    {

    public:
        static InstantTimer *create() { return new InstantTimer(); }
        InstantTimer();
        bool isComplete() override;
        bool reset() override { return true; }
    };

    class IntervalTimer : public Timer
    {
    public:
        static IntervalTimer *createByFrequency(size_t duration, TimerUnit unit = TIME_MSECS) { return create(1, duration, unit); }

        static IntervalTimer *createByRate(float ticsPerSecond) { return create(1, 1000.0 / ticsPerSecond); }

        static IntervalTimer *create(size_t tics = 0, size_t duration = 0, TimerUnit unit = TIME_MSECS);

        IntervalTimer(size_t duration = 0, TimerUnit unit = TIME_MSECS);
        virtual ~IntervalTimer();

        virtual bool isComplete() override;

    protected:
    };
} // namespace FCLIB

#endif // __TIMER_H__