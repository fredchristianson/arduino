#include <ArduinoJson.h>
#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{

    String HomeAssistant::nameToId(String name)
    {
        return nameToId(name.c_str());
    }
    String HomeAssistant::nameToId(const char *name)
    {
        return toUnfriendlyName(name);
    }

    String HomeAssistant::toUnfriendlyName(const char *name)
    {
        Logger log("HomeAssistant");
        log.debug("nameToId %s", name);

        String id;
        const char *p = name;
        while (*p != 0)
        {
            if (isalnum(*p))
            {
                id += (char)tolower(*p);
            }
            else if (*p == ' ')
            {
                id += '_';
            }
            p++;
        }
        log.debug("id %s", id.c_str());
        return id;
    }

    HomeAssistant::HomeAssistant(Mqtt *mqtt) : log("HomeAssistant")
    {
        log.debug("Create HomeAssistant");
        this->mqtt = mqtt;
        listener.handle(EventType::CONNECTED, mqtt, [this](Event *event)
                        { log.debug("MQTT connected %d %d", event->getId(), event->getType()); });
        listener.handle(EventType::DISCONNECTED, mqtt, [this](Event *event)
                        { log.debug("MQTT disconnected %d", event->getType()); });
    }

    HomeAssistant::~HomeAssistant()
    {
    }

    Device *HomeAssistant::createDevice(const char *name)
    {
        Device *dev = new Device(name);
        dev->ha = this;
        devices.add(dev);
        return dev;
    }

    void HomeAssistant::publishConfig()
    {
        log.info("publish entities");
        for (int didx = 0; didx < devices.size(); didx++)
        {
            Device *device = devices[didx];
            publishDeviceConfig(device);
        }
    }

    void HomeAssistant::publishDeviceConfig(Device *device)
    {
        for (int eidx = 0; eidx < device->entities.size(); eidx++)
        {
            Entity *entity = device->entities[eidx];
            publishEntityConfig(entity);
            entity->subscribe(mqtt);
            entity->publishState();
        }
    }
    void HomeAssistant::publishEntityConfig(Entity *entity)
    {

        String ename = entity->getComponentName();
        String topic = "homeassistant/" + ename + "/" + entity->id + "/config";
        JsonDocument doc;
        doc["name"] = entity->name;
        if (entity->deviceClass.length() > 0)
        {
            doc["device_class"] = entity->deviceClass;
        }
        doc["unique_id"] = entity->id;
        doc["~"] = entity->baseTopic();
        entity->setupCapabilities(doc);
        entity->setupStateTopic(doc);
        entity->setupCommandTopics(doc);
        doc["object_id"] = entity->id;
        JsonObject devObject = doc["device"].to<JsonObject>();
        entity->device->getJson(devObject);
        mqtt->send(topic, doc);
    }

    void HomeAssistant::publishState(const char *topic, JsonDocument &payload)
    {
        mqtt->send(topic, payload);
    }

    void HomeAssistant::publishState(const char *topic, const char *payload)
    {
        mqtt->send(topic, payload);
        mqtt->send(topic, payload);
    }
}