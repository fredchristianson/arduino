#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    MotionSensor::MotionSensor(Device *device, HW::Motion *hardware, const char *name) : BinarySensor(device, hardware, name)
    {
        deviceClass = "motion";
        events.handle(EventType::MOTION_START_EVENT, hardware, [this](Event *)
                      { this->onMotionStart(); });
        events.handle(EventType::MOTION_STOP_EVENT, hardware, [this](Event *)
                      { this->onMotionStop(); });
    }

    MotionSensor::~MotionSensor()
    {
    }

    void MotionSensor::onMotionStart()
    {
        log.debug("motion start");
        setStateOn();
    }
    void MotionSensor::onMotionStop()
    {
        log.debug("motion end");

        setStateOff();
    }
}
