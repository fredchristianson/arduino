#pragma once
#ifndef _FCLIB_THROTTLE_H_
#define _FCLIB_THROTTLE_H_
#include <cstddef>
#include "./Logging.h"
#include "./Callable.h"
#include "./System.h"
namespace FCLIB
{

    class Throttle
    {

    public:
        Throttle(size_t maxSpeed = 1, TimerUnit unit = TIME_SECONDS) : log("Throttle")
        {
            this->setMaxFrequency(maxSpeed, unit);
            this->lastCallbackMSecs = 0;
        }

        bool run(SimpleCallable callback)
        {
            if (lastCallbackMSecs + maxSpeedMSecs < THE_BOARD->currentMsecs())
            {
                callback();
                lastCallbackMSecs = THE_BOARD->currentMsecs();
                return true;
            }
            return false;
        }
        void setMaxFrequency(size_t maxSpeed = 1, TimerUnit unit = TIME_SECONDS)
        {
            this->maxSpeedMSecs = maxSpeed;
            if (unit == TIME_SECONDS)
            {
                this->maxSpeedMSecs *= 1000;
            }
            else if (unit == TIME_MINUTES)
            {
                this->maxSpeedMSecs = this->maxSpeedMSecs * 1000 * 60;
            }
        }

    protected:
        unsigned long lastCallbackMSecs;
        unsigned long maxSpeedMSecs;
        Logger log;
    };

} // namespace FCLIB

#endif // __TIMER_H__