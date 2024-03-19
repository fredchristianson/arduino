#include <WiFiManager.h>
#include <WiFiUdp.h>
#include "fclib/Net.h"
#include "fclib/Logging.h"
#include "fclib/EpochTime.h"

using namespace FCLIB;

namespace FCLIB
{
    TimeClient *singletonTimeClient = new TimeClient();
    WiFiUDP timeClientUdp;
    String lastDisplayTime;

    void TimeClient::run(long timezoneOffsetMinutes)
    {
        singletonTimeClient->setTimezoneOffset(timezoneOffsetMinutes);
        singletonTimeClient->start();
    }

    void TimeClient::setTimezoneOffsetMinutes(long minutes)
    {
        singletonTimeClient->setTimezoneOffset(minutes);
        singletonTimeClient->start();
    }

    long TimeClient::getTimezoneOffsetMinutes()
    {
        return singletonTimeClient->getTimezoneOffset();
    }

    TimeClient::TimeClient() : log("TimeClient")
    {
        this->ntp = NULL;
        this->timezoneOffsetMinutes = 0;
    }

    TimeClient::~TimeClient()
    {
        delete ntp;
    }

    const char *TimeClient::getFormattedTime()
    {
        if (this->ntp == NULL)
        {
            start();
        }
        lastDisplayTime = this->ntp->getFormattedTime();
        return lastDisplayTime.c_str();
    }

    long TimeClient::getTime()
    {
        if (this->ntp == NULL)
        {
            return 0; // todo: return board Time?
        }
        return this->ntp->getEpochTime();
    }
    void TimeClient::start()
    {
        if (this->ntp == NULL)
        {
            log.info("Start NTTPClient tzoffset: %d", this->timezoneOffsetMinutes);
            this->ntp = new NTPClient(timeClientUdp);
            this->ntp->begin();
            log.info("NTTPClient started");
        }
        this->ntp->setTimeOffset(this->timezoneOffsetMinutes * 60);
        this->ntp->update();
        EpochTime::getInstance().setSecondsNow(this->ntp->getEpochTime());
    }

    const char *TimeClient::getDisplayTime()
    {
        return singletonTimeClient->getFormattedTime();
    }

    long TimeClient::getEpochTime()
    {
        return singletonTimeClient->getTime();
    }
    void TimeClient::setTimezoneOffset(long minutes)
    {
        this->timezoneOffsetMinutes = minutes;
    }

}