#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    CommandEntity::CommandEntity(const char *name, ComponentType type) : Entity(name, type)
    {
        log.setModuleName("HA::CommandEntity");
    }

    CommandEntity::~CommandEntity()
    {
    }

    void CommandEntity::setupCommandTopics(JsonDocument &doc)
    {
        doc["command_topic"] = "~/command";
    }

    void CommandEntity::onCommand(const char *payload)
    {
        log.debug("got command: %s", payload);
    }

    void CommandEntity::subscribe(Mqtt *mqtt)
    {

        mqtt->subscribe(baseTopic() + "/command", [this](const char *payload)
                        { this->onCommand(payload); });
    }
}