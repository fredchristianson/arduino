#ifndef __FCNET_H_
#define __FCNET_H_
#include <ArduinoJson.h>
#include "fclib/Logging.h"
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include "fclib/Task.h"
#include "fclib/List.h"
#include "fclib/Config.h"

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
        List<WiFiPortalParameter> params;
    };

    class TimeClient
    {
    public:
        static TimeClient *run(long timezoneOffsetMinuetes = 0);
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
        using MqttCallback = std::function<void(const char *)>;
        Mqtt();
        virtual ~Mqtt();

        bool configure(const char *deviceName, const char *mqttServer, const char *user, const char *password, int mqttPort = 1883);

        bool connect();

        void send(String topic, JsonDocument &doc, bool noLog = false);
        void send(String topic, const char *, bool noLog = false);

        void subscribe(const char *topic, MqttCallback callback);
        void subscribe(const String &topic, MqttCallback callback) { subscribe(topic.c_str(), callback); }

        void startLogger(const char *topic, LogLevel level);
        void checkConnection();

    protected:
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

        class Subscriber
        {
        public:
            Subscriber(const char *stopic, MqttCallback callback)
            {
                Logger log("MqttSubscriber");
                log.debug("topic: %s", stopic);
                this->topic = stopic;
                this->callback = callback;
                log.debug("this->topic: %s", this->topic.c_str());
            }
            String topic;
            MqttCallback callback;
        };

        class LogDestination : public ILogDestination
        {
        public:
            LogDestination(Mqtt *mqtt, const char *topic, LogLevel level);
            virtual void write(int level, const char *message) const;

        protected:
            String topic;
            int level;
            Mqtt *mqtt;
        };

        List<Subscriber>
            subscribers;
    };

    class Network
    {
    public:
        static Mqtt *getMqtt();
        Network();
        virtual ~Network();

        virtual bool setup(Config *config);
        virtual bool connect();

    protected:
        bool connectWifi();
        bool connectMqtt();
        bool connectTimeClient();
        bool checkConnection();

        String mqttUser;
        String mqttPassword;
        String mqttServer;
        String mqttDeviceName;
        bool resetWifi;
        int ntpOffsetMinutes;
        String deviceName;

    private:
        static Mqtt *singletonMqtt;
        static TimeClient *singletonTimeClient;
        static Network *singletonNetwork;
        Logger log;
    };
}

#endif