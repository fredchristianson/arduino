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
            this->log.never("in calback %s %s %d", topic, message.c_str(), len);
            message.concat((const char *)msg, len);
            handleMessage(messageTopic, message);
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

    void Mqtt::send(String topic, JsonDocument &doc)
    {
        checkConnection();
        int len = measureJson(doc);
        pubSubClient.beginPublish(topic.c_str(), len, false);
        serializeJson(doc, pubSubClient);
        pubSubClient.endPublish();
    }

    void Mqtt::send(String topic, const char *payload)
    {
        checkConnection();
        pubSubClient.publish(topic.c_str(), payload);
    }

    void Mqtt::checkConnection()
    {
        if (!pubSubClient.connected())
        {
            this->reconnect();
        }
        pubSubClient.loop();
    }

    void Mqtt::handleMessage(String &topic, String &message)
    {
        log.never("handle message: %s", topic.c_str());
        for (int i = 0; i < subscribers.size(); i++)
        {
            Subscriber *sub = subscribers[i];
            log.never("\tcheck subscriber: %lx %s ", sub, sub->topic.c_str());
            if (topic.equals(subscribers[i]->topic))
            {
                log.never("\t\tcallback");
                subscribers[i]->callback(message.c_str());
            }
        }
    }

    void Mqtt::subscribe(const char *topic, MqttCallback callback)
    {
        log.never("subscribe to: %s", topic);
        Subscriber *sub = new Subscriber(topic, callback);
        subscribers.add(sub);
        pubSubClient.subscribe(topic);
        log.never("subscriber topic: %lx %s", sub, sub->topic.c_str());
    }
}