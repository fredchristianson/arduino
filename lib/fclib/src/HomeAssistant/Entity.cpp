#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Entity::Entity(const char *name, Device *device, ComponentType type) : log("HA::Entity")
    {
        this->name = name;
        this->type = type;
        this->device = device;
        componentTypeIndex = 0;
        defaultEntity = false;
        device->add(this);
        lastBoolState = false;
    }

    Entity::~Entity()
    {
        device->remove(this);
    }

    HomeAssistant *Entity::ha() { return device->ha; }

    String Entity::baseTopic()
    {
        return "homeassistant/" + componentName + "/" + id;
    }

    void Entity::publishState()
    {
        JsonDocument doc;
        doc["state"] = lastBoolState ? "ON" : "OFF";
        ha()->publishState(baseTopic() + "/set", doc);
    }

    void Entity::setBoolState(bool isOn)
    {
        lastBoolState = isOn;

        this->publishState();
    }

    void Entity::setStateOn()
    {
        setBoolState(true);
    }

    void Entity::setStateOff()
    {
        setBoolState(false);
    }

    void Entity::setupCapabilities(JsonDocument &doc)
    {
        // no defaults
    }

    void Entity::setupStateTopic(JsonDocument &doc)
    {
        doc["state_topic"] = "~/set";
        doc["schema"] = "json";
        // seems like value_template is used, not state_value_template
        doc["state_value_template"] = "{{ value_json.state }}";
        doc["value_template"] = "{{ value_json.state }}";
        // doc["payload_on"] = "on";
        // doc["payload_off"] = "off";

        // only "ON" and "OFF" work
        doc["state_on"] = "ON";
        doc["state_off"] = "OFF";
    }
    void Entity::setupCommandTopics(JsonDocument &doc)
    {
        // no default topics
    }
}
