#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Led::Led(Device *device, HW::OutputPinComponent *hardware, const char *name) : Entity(name == NULL ? "Button" : name, device, ComponentType::LIGHT)
    {
        this->hardware = hardware;
        deviceClass = "led";
        componentName = "light";
    }

    Led::~Led()
    {
    }

}
