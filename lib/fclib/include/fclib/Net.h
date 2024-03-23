#ifndef __FCNET_H_
#define __FCNET_H_
#include "fclib/Logging.h"
#include "fclib/LinkedList.h"
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include "fclib/Task.h"

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

    class Mqtt
    {
    public:
        Mqtt();
        virtual ~Mqtt();

        bool configure(const char *deviceName, const char *mqttServer, const char *user, const char *password, int mqttPort = 1883);

        bool reconnect();

    protected:
        void checkConnection();
        void handleMessage(String &topic, String &message);
        WiFiClient wifiClient;
        PubSubClient pubSubClient;
        Logger log;
        String mqttServer;
        String deviceName; // primary device if multiple presented to HA
        String deviceId;
        String user;
        String password;
        int port;
    };
}

#endif