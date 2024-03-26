#include "fclib/HomeAssistant.h"
#include <ArduinoJson.h>
using namespace FCLIB;

namespace FCLIB::HA
{
    Switch::Switch(Device *device, HW::InputPinComponent *hardware, const char *name) : Entity(name == NULL ? "Button" : name, device, ComponentType::SWITCH)
    {
        log.setModuleName("HA::Switch");
        log.debug("Switch created 0x%lx", hardware);
        this->hardware = hardware;
        deviceClass = "switch";
        componentName = "switch";
        events.handle(EventType::CHANGE_EVENT, hardware, [this](Event *)
                      { 
                        log.debug("setup");
                        this->onHardwareChange(); });
    }

    Switch::~Switch()
    {
    }

    void Switch::onHardwareChange()
    {
        bool isSet = hardware->getState() != 0;
        log.debug("Switch state change %d", isSet);
        // JsonDocument doc;
        // doc["state"] = isSet ? "on" : "off";
        ha()->publishState(this, isSet ? "on" : "off");
    }
}
