#include <cstddef>
#include "./Logging.h"

#ifndef __TIMER_H__
#define __TIMER_H__

namespace FCLIB
{
    class Timer
    {
    public:
        static Timer *createByFrequency(size_t msecs) { return create(1, msecs); }

        static Timer *createByRate(float ticsPerSecond) { return create(1, 1000.0 / ticsPerSecond); }

        static Timer *create(size_t tics, size_t durationMsecs);

    public:
        virtual bool isComplete();
        virtual bool reset();
        virtual void destroy() { delete this; }

    protected:
        Timer(size_t durationMsecs);
        virtual ~Timer();

        unsigned long currentTimeMsecs();

        unsigned long startTimeMsecs;
        unsigned long durationMsecs;
        Logger LOG;
    };

    class IntervalTimer : public Timer
    {
    public:
        static IntervalTimer *createByFrequency(size_t msecs) { return create(1, msecs); }

        static IntervalTimer *createByRate(float ticsPerSecond) { return create(1, 1000.0 / ticsPerSecond); }

        static IntervalTimer *create(size_t tics, size_t durationMsecs);

        virtual bool isComplete() override;

    protected:
        IntervalTimer(size_t msecs);
        virtual ~IntervalTimer() override;
    };
} // namespace FCLIB

#endif // __TIMER_H__