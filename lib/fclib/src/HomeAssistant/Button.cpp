#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Button::Button(const char *name) : Entity(name == NULL ? "Button" : name, ComponentType::BUTTON)
    {
        deviceClass = "button";
        componentName = "binary_sensor";
    }

    Button::~Button()
    {
    }

}
