#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Button::Button(Device *device, const char *name) : Entity(name == NULL ? "Button" : name, device, ComponentType::BUTTON)
    {
        deviceClass = "button";
        componentName = "binary_sensor";
    }

    Button::~Button()
    {
    }

}
