#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    BinarySensor::BinarySensor(HW::InputPinComponent *hardware, const char *name) : Entity(name == NULL ? "Button" : name, ComponentType::BINARY_SENSOR)
    {
        this->hardware = hardware;
        deviceClass = "Running";
        componentName = "binary_sensor";
    }

    BinarySensor::~BinarySensor()
    {
    }

}
