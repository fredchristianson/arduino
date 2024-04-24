#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    MotionSensor::MotionSensor(HW::IMotion *hardware, const char *name) : BinarySensor(hardware, name)
    {
        deviceClass = "motion";
        paused = false;
        events.handle(EventType::MOTION_START_EVENT, hardware, [this](Event *)
                      { this->onMotionStart(); });
        events.handle(EventType::MOTION_STOP_EVENT, hardware, [this](Event *)
                      { this->onMotionStop(); });
        events.handle(EventType::SCENE_START_TRANSITION, hardware, [this](Event *)
                      { paused = true; });
        events.handle(EventType::SCENE_END_TRANSITION, hardware, [this](Event *)
                      { Task::once([this]()
                                   { paused = false; })
                            ->delaySeconds(1); });
    }

    MotionSensor::~MotionSensor()
    {
    }

    void MotionSensor::onMotionStart()
    {
        if (paused)
        {
            return;
        }
        log.debug("motion start");
        setStateOn();
    }
    void MotionSensor::onMotionStop()
    {
        if (paused)
        {
            return;
        }
        log.debug("motion end");

        setStateOff();
    }
}
