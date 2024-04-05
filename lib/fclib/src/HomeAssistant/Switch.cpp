#include "fclib/HomeAssistant.h"
#include <ArduinoJson.h>
using namespace FCLIB;

namespace FCLIB::HA
{
    Switch::Switch(HW::InputPinComponent *hardware, const char *name) : Entity(name == NULL ? "Button" : name, ComponentType::SWITCH)
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
        setBoolState(isSet);
    }
}
