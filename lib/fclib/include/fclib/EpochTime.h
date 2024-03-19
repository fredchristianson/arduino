#ifndef EPOCH_TIME_H
#define EPOCH_TIME_H

#include "./System.h"
#include <HardwareSerial.h>
using namespace FCLIB;

namespace FCLIB
{
    // setSecondsNow() must be called with the Epoch time before this
    // is accurate.  Otherwise it returns time since restart
    // sets the epoch time once and return hour:minute:seconds
    // any time after, updated with change in millis()
    class EpochTime
    {
    public:
        static EpochTime &getInstance();
        EpochTime()
        {
            m_secondsAtMillis = 0;
            m_millis = 0;
            m_gmtOffsetMinutes = 0;
        }

        void setGmtOffsetMinutes(int offsetMinutes)
        {
            m_gmtOffsetMinutes = offsetMinutes;
        }

        int getGmtOffsetMinutes() const { return m_gmtOffsetMinutes; }

        void setSecondsNow(unsigned long seconds)
        {
            m_millis = THE_BOARD->currentMsecs();
            m_secondsAtMillis = seconds;
        }

        unsigned long getHour()
        {
            long h = getEpochSeconds();
            return (h / 3600) % 24;
        }

        unsigned long getMinute()
        {
            long h = getEpochSeconds();
            return (h / 60) % 60;
        }

        unsigned long getSecond()
        {
            long h = getEpochSeconds();
            return (h) % 60;
        }

        unsigned long getEpochSeconds()
        {

            unsigned long diff = THE_BOARD->currentMsecs() - m_millis;
            unsigned long seconds = m_secondsAtMillis + m_gmtOffsetMinutes * 60 + (diff / 1000);
            return seconds;
        }

    private:
        unsigned long m_gmtOffsetMinutes;
        unsigned long m_secondsAtMillis;
        unsigned long m_millis;
    };

}
#endif