#ifndef __FCNET_H_
#define __FCNET_H_
#include "fclib/Logging.h"
#include "fclib/LinkedList.h"
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <NTPClient.h>

namespace FCLIB
{
    class WiFiPortalParameter : public WiFiManagerParameter
    {
    public:
        WiFiPortalParameter(
            const char *name,
            const char *prompt,
            const char *defaultValue,
            int len) : WiFiManagerParameter(name, prompt, defaultValue, len)
        {
        }
    };

    class WiFiSetup
    {
    public:
        WiFiSetup();
        ~WiFiSetup();

        bool connect(const char *name);
        bool startPortal(const char *name);

        const char *getIP();

        void addParameter(WiFiPortalParameter &param);
        void setSaveConfigCallback(void (*callback)(WiFiSetup *));

    private:
        Logger log;
        LinkedList<WiFiPortalParameter *> params;
    };

    class TimeClient
    {
    public:
        static void run(long timezoneOffsetMinuetes = 0);
        static const char *getDisplayTime();
        static long getEpochTime();
        static void setTimezoneOffsetMinutes(long minutes);
        static long getTimezoneOffsetMinutes();

        TimeClient();
        virtual ~TimeClient();
        void start();

        const char *getFormattedTime();
        long getTime();
        void setTimezoneOffset(long minutes);
        long getTimezoneOffset() { return this->timezoneOffsetMinutes; }

    protected:
        NTPClient *ntp;
        Logger log;
        long timezoneOffsetMinutes;
    };
}

#endif