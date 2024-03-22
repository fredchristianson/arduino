#include "fclib/Net.h"

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

    bool Mqtt::connect(const char *mqttServer, const char *deviceName, const char *user, const char *password, int port)
    {
        pubSubClient.setServer(mqttServer, port);
        log.info("MQTT connect: %s %s %s %s", mqttServer, deviceName, user, password);
        if (!pubSubClient.connect(deviceName, user, password))
        {
            log.error("MQTT failed to connect. State %d", pubSubClient.state());
            return false;
        }

        bool sent = pubSubClient.publish("test_message", "this is the test");
        log.always("Publish result %d", sent);
        return true;
    }
}