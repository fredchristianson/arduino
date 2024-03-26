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
    }

    Entity::~Entity()
    {
        device->remove(this);
    }

    HomeAssistant *Entity::ha() { return device->ha; }
}
