#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    MotionSensor::MotionSensor(HW::IMotion *hardware, const char *name) : BinarySensor(hardware, name), pause(5, TimerUnit::TIME_SECONDS)
    {
        log.setModuleName("HA::MotionSensor");
        deviceClass = "motion";
        events.handle(EventType::MOTION_START_EVENT, hardware, [this](Event *)
                      { this->onMotionStart(); });
        events.handle(EventType::MOTION_STOP_EVENT, hardware, [this](Event *)
                      { this->onMotionStop(); });
        events.handle(EventType::SCENE_START_TRANSITION, [this](Event *)
                      { pause.reset(1, TimerUnit::TIME_MINUTES); });
        events.handle(EventType::SCENE_END_TRANSITION, [this](Event *)
                      { pause.reset(5, TimerUnit::TIME_SECONDS); });
        pause.reset(10, TimerUnit::TIME_SECONDS); // wait for things to start and motion clear before sending to HA
    }

    MotionSensor::~MotionSensor()
    {
    }

    void MotionSensor::onMotionStart()
    {
        if (!pause.isComplete())
        {
            return;
        }
        log.always("motion start");
        setStateOn();
    }
    void MotionSensor::onMotionStop()
    {

        log.always("motion end");

        setStateOff();
    }
}
