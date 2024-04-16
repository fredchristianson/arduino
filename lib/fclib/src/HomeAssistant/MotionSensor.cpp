#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    MotionSensor::MotionSensor(HW::IMotion *hardware, const char *name) : BinarySensor(hardware, name)
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
