
#include "./HallApp.h"
#include "fclib/Hardware.h"
#include "fclib/Timer.h"
#include "fclib/LedStrip.h"
#include "fclib/Render.h"

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
    // ledStrip.setPin(2);

    listener.handle(EventType::PRESS_EVENT, &button, [this](Event *event)
                    { this->onButtonChange(event); });

    String deviceName = getConfig()->get("device_name", "FCLIB Device");
    ha = new HA::HomeAssistant(getNetwork()->getMqtt());
    haDevice = ha->createDevice(deviceName.c_str());
    // haButton = new HA::Switch(haDevice, &button, "Toggle Switch");
    // haLed = new HA::Led(haDevice, &led);

    NeoPixelStrip *strip = new NeoPixelStrip(5, 15, 100);
    this->ledStrip = strip;
    ha->publishConfig();

    Task::repeat([this]()
                 { this->log.showMemory(); })
        ->delaySeconds(60);
}

int r = 0;
int g = 0;
int b = 0;

void HallApp::doTask()
{
    ledStrip->clear();
    log.debug("render");
    RGBRenderer rgb(r, g, b);
    r += 1;
    g += 2;
    b += 3;
    rgb.draw(*this->ledStrip);
    // HSVRenderer hsv(100, 100, 50);
    // hsv.draw(*this->ledStrip);
    ledStrip->show();
}

void HallApp::onButtonChange(Event *event)
{
    log.never("handle event 0x%lx %d", event, event->getType());

    if (event->getSender() == &(button))
    {
        // led.toggle();
        log.debug("got button event");
    }
}