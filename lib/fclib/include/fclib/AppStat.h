#pragma once
#ifndef _FCLIB_APP_STAT_H
#define _FCLIB_APP_STAT_H
#include <c_types.h>
#include <stdlib.h>
#include "fclib/Logging.h"

namespace FCLIB
{

    class AppStat
    {
    public:
        AppStat(const char *name);
        virtual ~AppStat();
        virtual void write();
        virtual void reset() = 0;

    protected:
        virtual void writeDetails() = 0;
        String name;
        bool inUse;
        static Logger statLogger;
        Logger &log;
    };

    class AppCounterStat : public AppStat
    {
    public:
        AppCounterStat(const char *name);
        virtual ~AppCounterStat();
        void increment();
        void decrement();
        void add(int amount);
        void writeDetails() override;
        void reset() override;

    protected:
        int count;
    };

    class AppMemoryStat : public AppStat
    {
    public:
        AppMemoryStat(const char *name);
        virtual ~AppMemoryStat();
        void update();
        void writeDetails() override;
        void reset() override;

    protected:
        long count;
        double heapSum;
        long heapMin;
        long heapMax;
        double stackSum;
        long stackMin;
        long stackMax;
    };

    class AppRangeStat : public AppStat
    {
    public:
        AppRangeStat(const char *name);
        virtual ~AppRangeStat();
        void value(float value);
        void writeDetails() override;
        void reset() override;

    protected:
        int count;
        float minValue;
        float maxValue;
        float currentValue;
        float sum;
    };

    class AppTimerStat : public AppRangeStat
    {
    public:
        AppTimerStat(const char *name);
        virtual ~AppTimerStat();
        void start();
        void end();
        void writeDetails() override;
        void reset() override;

    protected:
        int startMsecs;
    };

}
#endif
