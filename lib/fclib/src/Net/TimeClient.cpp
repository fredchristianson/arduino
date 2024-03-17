#include <WiFiManager.h>
#include <WiFiUdp.h>
#include "fclib/Net.h"
#include "fclib/Logging.h"

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
            log.info("Start NTTPClient");
            this->ntp = new NTPClient(timeClientUdp);
            this->ntp->setTimeOffset(this->timezoneOffsetMinutes);
            this->ntp->begin();
            log.info("NTTPClient started");
        }
        this->ntp->update();
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
        this->start();
        this->ntp->setTimeOffset(minutes);
        this->ntp->update();
    }

    void TimeClient::setTimezoneOffsetMinutes(long minutes)
    {
        singletonTimeClient->start();
        singletonTimeClient->setTimezoneOffset(minutes);
    }

    long TimeClient::getTimezoneOffsetMinutes()
    {
        return singletonTimeClient->getTimezoneOffset();
    }
}