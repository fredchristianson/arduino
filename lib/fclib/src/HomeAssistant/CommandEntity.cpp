#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    CommandEntity::CommandEntity(const char *name, Device *device, ComponentType type) : Entity(name, device, type)
    {
        log.setModuleName("HA::CommandEntity");
        device->add(this);
    }

    CommandEntity::~CommandEntity()
    {
        device->remove(this);
    }

    void CommandEntity::setupCommandTopics(JsonDocument &doc)
    {
        doc["command_topic"] = "~/command";
    }

    void CommandEntity::onCommand(const char *payload)
    {
        log.always("got command: %s", payload);
    }

    void CommandEntity::subscribe(Mqtt *mqtt)
    {

        mqtt->subscribe(baseTopic() + "/command", [this](const char *payload)
                        { this->onCommand(payload); });
    }
}