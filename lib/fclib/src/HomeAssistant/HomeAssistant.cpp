#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    HomeAssistant::HomeAssistant(Mqtt *mqtt) : log("HomeAssistant")
    {
        this->mqtt = mqtt;
        listener.handle(EventType::CONNECTED, mqtt, [this](Event *)
                        { log.debug("MQTT connected"); });
        listener.handle(EventType::DISCONNECTED, mqtt, [this](Event *)
                        { log.debug("MQTT disconnected"); });
    }

    HomeAssistant::~HomeAssistant()
    {
    }
}
/*
    void Mqtt::publishDiscovery()
    {
        for (int i = 0; i < devices.size(); i++)
        {
            publishDiscovery(devices[i]);
        }
    }

    void Mqtt::publishDiscovery(HA::Device *device)
    {
        String discoveryTopic = "homeassistant/button/" + hanam(deviceName) + "/config";
        String payload = "{\"stat_t\": \"test\", \"name\": \"" + deviceName + "\", \"unique_id\": \"" + hanam(deviceId) + "\", \"device\": {\"name\": \"xyz\", \"identifiers\":[\"123\",\"abc\"]} }";
        pubSubClient.publish(discoveryTopic.c_str(), payload.c_str());
        log.debug("send discovery: %s\n%s", discoveryTopic.c_str(), payload.c_str());
    }
*/