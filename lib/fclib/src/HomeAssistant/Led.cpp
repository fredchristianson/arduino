#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Led::Led(Device *device, HW::OutputPinComponent *hardware, const char *name) : Entity(name == NULL ? "Button" : name, device, ComponentType::LIGHT)
    {
        log.setModuleName("HA::Led");
        this->hardware = hardware;
        deviceClass = "led";
        componentName = "light";
    }

    Led::~Led()
    {
    }

    void Led::updateState(const char *payload)
    {
        String val = payload;
        log.debug("updateState: %s", payload);
        bool on = val == "on" || val.indexOf("\"ON\"") >= 0;
        if (on == hardware->getState())
        {
            log.debug("no change");
            return; // no change
        }
        log.debug("Change LED state: %s", on ? "on" : "off");
        hardware->setOn(on);
        ha()->publishState(this, on ? "on" : "off");
    }
}
