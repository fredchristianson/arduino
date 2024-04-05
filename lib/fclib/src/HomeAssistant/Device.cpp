#include "fclib/HomeAssistant.h"
#include "fclib/HomeAssistant.h"
#include "fclib/System.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Device::Device(const char *name) : log("HA::Device")
    {
        this->friendlyName = name;
        this->name = HomeAssistant::toUnfriendlyName(name);
        this->boardId = THE_BOARD->getDeviceId();
        log.debug("name: %s", this->name.c_str());
        log.debug("board ID %s", this->boardId.c_str());
        this->id = HomeAssistant::nameToId(this->name + "_" + this->boardId);
    }

    Device::~Device()
    {
    }

    void Device::add(Entity *entity)
    {
        log.debug("add entity: %x %s", entity, entity->getUniqueName());
        entity->device = this;
        entity->componentTypeIndex = 0;
        for (int i = 0; i < entities.size(); i++)
        {
            if (entity->type == entities[i]->type)
            {
                entity->componentTypeIndex++;
            }
        }
        entity->uniqueName = entity->name + ":" + String(entity->componentTypeIndex);
        if (entities.size() == 0)
        {
            entity->defaultEntity = true;
            entity->id = this->id;
        }
        else
        {
            entity->defaultEntity = false;
            entity->id = HomeAssistant::nameToId(entity->name + "_" + this->boardId);
        }
        entities.add(entity);
    }

    void Device::remove(Entity *entity)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            if (entities[i] == entity)
            {
                entities.remove(i);
                return;
            }
        }
    }
}

void FCLIB::HA::Device::getJson(JsonObject &json)
{
    ConfigSection *cfg = App::THE_APP->getConfig()->getSection("default");
    json["name"] = friendlyName.c_str();
    json["manufacturer"] = "Fred Christianson";
    json["model"] = cfg->get("model", "fc device");
    json["serial_number"] = id;
    json["hw_version"] = cfg->get("hw_version", "x.x.x");
    json["sw_version"] = cfg->get("sw_version", "x.x.x");
    JsonArray ids = json["identifiers"].to<JsonArray>();
    ids.add(id);
}