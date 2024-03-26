
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
    LoopTask::create([this]()
                     { this->doTask(); });

    button.setPin(4);
    led.setPin(5);

    listener.handle(EventType::PRESS_EVENT, &button, [this](Event *event)
                    { this->onButtonChange(event); });

    String deviceName = getConfig()->get("device_name", "FCLIB Device");
    ha = new HA::HomeAssistant(getNetwork()->getMqtt());
    haDevice = ha->createDevice(deviceName.c_str());
    haButton = new HA::Switch(haDevice, &button, "Toggle Switch");
    haLed = new HA::Led(haDevice, &led);

    ha->publishConfig();

    Task::repeat([this]()
                 { this->log.showMemory(); })
        ->delaySeconds(60);
}

void HallApp::doTask()
{
}

void HallApp::onButtonChange(Event *event)
{
    log.never("handle event 0x%lx %d", event, event->getType());

    if (event->getSender() == &(button))
    {
        led.toggle();
        log.debug("got button event");
    }
}