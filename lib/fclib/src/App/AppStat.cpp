#include <stdlib.h>
#include "fclib/AppStat.h"
#include "fclib/App.h"

namespace FCLIB
{
    Logger AppStat::statLogger("AppStat");

    AppStat::AppStat(const char *name) : log(AppStat::statLogger)
    {
        this->name = name;
        inUse = true;
        App::THE_APP->addStat(this);
    }

    AppStat::~AppStat()
    {
        App::THE_APP->removeStat(this);
    }

    void AppStat::write()
    {
        log.info(name.c_str());
        LogIndent indent;
        writeDetails();
    }

    AppCounterStat::AppCounterStat(const char *name) : AppStat(name)
    {
        reset();
    }

    AppCounterStat::~AppCounterStat()
    {
    }
    void AppCounterStat::writeDetails()
    {
        log.info("count: %d", count);
    }
    void AppCounterStat::reset()
    {
        count = 0;
    }

    void AppCounterStat::increment() { count++; }
    void AppCounterStat::decrement() { count--; }
    void AppCounterStat::add(int amount) { count += amount; }

    AppMemoryStat::AppMemoryStat(const char *name) : AppStat(name)
    {
        reset();
    }

    AppMemoryStat::~AppMemoryStat()
    {
    }
    void AppMemoryStat::writeDetails()
    {
        update();
        long s = THE_BOARD->getFreeContStack();
        long h = THE_BOARD->getFreeHeap();
        log.info("Heap\tcurrent=%d\tmin=%d\tmax=%d\tavg=%.2f", h, heapMin, heapMax, heapSum / (count == 0 ? 1 : count));
        log.info("Stack\tcurrent=%d\tmin=%d\tmax=%d\tavg=%.2f", s, stackMin, stackMax, stackSum / (count == 0 ? 1 : count));
    }
    void AppMemoryStat::reset()
    {
        count = 0;
        heapSum = 0;
        heapMax = 0;
        heapMin = 0;
        stackSum = 0;
        stackMin = 0;
        stackMax = 0;
    }

    void AppMemoryStat::update()
    {
        long s = THE_BOARD->getFreeContStack();
        long h = THE_BOARD->getFreeHeap();
        if (count == 0)
        {
            count = 1;
            heapMax = h;
            heapMin = h;
            heapSum = h;
            stackSum = s;
            stackMin = s;
            stackMax = s;
        }
        else
        {
            count += 1;
            heapSum += h;
            heapMin = min(heapMin, h);
            heapMax = max(heapMax, h);
            stackSum += s;
            stackMin = min(stackMin, s);
            stackMax = max(stackMax, s);
        }
    }

    AppRangeStat::AppRangeStat(const char *name) : AppStat(name)
    {
        reset();
    }

    AppRangeStat::~AppRangeStat()
    {
    }
    void AppRangeStat::writeDetails()
    {
        log.info("value=%.2f\tmin=%.2f\tmax=%.2f\tavg=%.2f", currentValue, minValue, maxValue, sum / (count == 0 ? 1 : count));
    }

    void AppRangeStat::value(float val)
    {
        if (count == 0)
        {
            count = 1;
            sum = val;
            minValue = val;
            maxValue = val;
        }
        else
        {
            count++;
            sum += val;
            minValue = min(minValue, val);
            maxValue = max(maxValue, val);
        }
    }
    void AppRangeStat::reset()
    {
        count = 0;
        minValue = 0;
        maxValue = 0;
        currentValue = 0;
        sum = 0;
    }

    AppTimerStat::AppTimerStat(const char *name) : AppRangeStat(name)
    {
        reset();
    }

    AppTimerStat::~AppTimerStat()
    {
    }
    void AppTimerStat::writeDetails()
    {
        log.info("last=%.2fmsecs\tmin=%.2fmsecs\tmax=%.2fmsecs\tavg=%.2fmsec", currentValue, minValue, maxValue, sum / (count == 0 ? 1 : count));
    }

    void AppTimerStat::start()
    {
        this->startMsecs = THE_BOARD->currentMsecs();
    }

    void AppTimerStat::end()
    {
        int endMsecs = THE_BOARD->currentMsecs();
        value(endMsecs - startMsecs);
    }
    void AppTimerStat::reset()
    {
        startMsecs = 0;
        AppRangeStat::reset();
    }
}