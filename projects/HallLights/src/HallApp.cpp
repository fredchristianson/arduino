
#include "./HallApp.h"
#include "fclib/Hardware.h"
#include "fclib/Timer.h"

using namespace FCLIB;

HallApp::HallApp()
{
    log.setModuleName("HallApp");

    log.info("Created");
}

HallApp::~HallApp()
{
}

void HallApp::setupComplete()
{
    halfMinute.seconds(30);
    LoopTask::create([this]()
                     { this->doTask(); });

    button.setPin(4);
    led.setPin(5);

    listener.handle(EventType::CHANGE_EVENT, &button, [this](Event *event)
                    { this->onButtonChange(event); });

    String deviceName = getConfig()->get("device_name", "FCLIB Device");
    ha = new HA::HomeAssistant(getNetwork()->getMqtt());
    haDevice = new HA::Device(deviceName.c_str());
    haButton = new HA::BinarySensor(haDevice, &button);
    haLed = new HA::Led(haDevice, &led);

    Task::repeat([this]()
                 { this->log.showMemory(); })
        ->delaySeconds(60);
}

void HallApp::doTask()
{
    if (button.isPressed())
    {
        // log.debug("pressed");
    }
    if (halfMinute.isComplete())
    {
        log.showMemory();
    }
}

void HallApp::onButtonChange(Event *event)
{
    log.debug("handle event 0x%lx %d", event, event->getType());

    if (event->getSender() == &(button))
    {
        led.toggle();
        log.debug("got button event");
    }
}