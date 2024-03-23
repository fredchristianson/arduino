// vscode needs this #define or it shows errors
#ifndef ESP8266
#define ESP8266
#endif

#include "fclib/Net.h"
#include "fclib/Task.h"
#include "fclib/Timer.h"
#include "fclib/System.h"
#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{

    Mqtt::Mqtt() : pubSubClient(wifiClient), log("Mqtt")
    {
    }

    Mqtt::~Mqtt()
    {
        pubSubClient.disconnect();
    }

    // primary device if multiple presented to HA
    bool Mqtt::configure(const char *mqttServer, const char *deviceName, const char *user, const char *password, int port)
    {
        this->mqttServer = mqttServer;
        this->deviceName = deviceName;
        this->deviceId = this->deviceName + ":" + THE_BOARD->getDeviceId();
        this->user = user;
        this->password = password;
        this->port = port;

        Task::repeat([this]()
                     { this->checkConnection(); })
            ->delayMsecs(10);

        auto callback = [this](char *topic, uint8_t *msg, unsigned int len)
        {
            String messageTopic(topic);
            String message;
            message.concat((const char *)msg, len);
            handleMessage(messageTopic, message);
            this->log.debug("in callback %s %s %d", topic, message.c_str(), len);
        };

        pubSubClient.setCallback(callback);

        this->reconnect();
        return true;
    }

    bool Mqtt::reconnect()
    {
        while (!pubSubClient.connected())
        {
            pubSubClient.setServer(mqttServer.c_str(), port);
            log.info("MQTT connect: %s %s %s %s", mqttServer.c_str(), deviceName.c_str(), user.c_str(), password);
            if (!pubSubClient.connect(deviceName.c_str(), user.c_str(), password.c_str()))
            {
                Event::trigger(EventType::DISCONNECTED, this);

                log.error("MQTT failed to connect. State %d", pubSubClient.state());
            }
            else
            {
                Event::trigger(EventType::CONNECTED, this);
            }
        }
        return true;
    }

    void Mqtt::checkConnection()
    {
        if (!pubSubClient.connected())
        {
            this->reconnect();
        }
        pubSubClient.loop();
    }

    void handleMessage(String &topic, String &message)
    {
    }

}