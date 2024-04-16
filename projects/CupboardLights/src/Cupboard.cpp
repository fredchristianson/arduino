
#include "./CupboardApp.h"
#include "fclib/Hardware.h"
#include "fclib/Timer.h"
#include "fclib/LedStrip.h"
#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/Net.h"

using namespace FCLIB;

CupboardApp::CupboardApp()
{
    log.setModuleName("CupboardApp");

    log.showMemory("CupboardApp Created");
}

CupboardApp::~CupboardApp()
{
}

void CupboardApp::setupComplete()
{
    Task::onLoop([this]()
                 { this->doTask(); });
    Task::repeat([this]()
                 { this->log.showMemory(); })
        ->delaySeconds(5);

    Config *config = getConfig();
    motion.addPin(config->get("motion1", "pin", -1));
    motion.addPin(config->get("motion2", "pin", -1));
    strip1.setPin(config->get("led1", "pin", -1));
    strip2.setPin(config->get("led2", "pin", -1));
    strip1.setCount(config->get("led1", "led_count", -1));
    strip2.setCount(config->get("led2", "led_count", -1));
    strips.setOrig(&strip1);
    strips.setCopy(&strip2);
    log.showMemory("setup listeners");
    listener.handle(EventType::CHANGE_EVENT, &motion, [this](Event *event)
                    { this->onMotionChange(event); });

    listener.handleChange(config, [this, config](Event *event)
                          {
               this->log.debug("save config");
               config->save(); });
}

void CupboardApp::doTask()
{

    GradientRenderer solid(Color::fromRGB(255, 0, 120), Color::fromRGB(0, 255, 0));
    solid.draw(strips);

    strips.show();
}

void CupboardApp::onMotionChange(Event *event)
{
    log.always("handle event 0x%lx %d", event, event->getType());
    log.always("Motion %s detected.", motion.isDetected() ? " is " : " is not ");
}
