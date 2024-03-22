
#include "./HallApp.h"
#include "fclib/Component.h"
#include "fclib/Timer.h"

using namespace FCLIB;
using namespace FCLIB::COMPONENT;

class TestTaskAction : public TaskAction
{
public:
    TestTaskAction(const char *name) : log(name)
    {
        this->name = name;
    }
    virtual void doTask()
    {
        log.always("running TestTaskAction: %s", name.c_str());
    }

protected:
    String name;
    Logger log;
};

TestTaskAction once("Once");
TestTaskAction repeat("Repeat Forever");
TestTaskAction onceA("OnceA");
TestTaskAction repeatA("Repeat A Forever");
TestTaskAction repeatB("Repeat 4 times");

HallApp::HallApp() : listener(this)
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
    LoopTask::create(this);

    // Task::once(once)->delaySeconds(5);
    // Task::once(onceA)->delaySeconds(15);
    // Task::once(once)->delaySeconds(7)->delayMinutes(1);
    // Task::once(once)->delayMsecs(5);
    // Task::once(onceA)->delayMsecs(5);

    // Task::repeat(repeat)->delayMinutes(1);
    // Task::repeat(repeatA, 100)->delaySeconds(13);
    // Task::repeat(repeatB, 4)->delayMsecs(750);
    button.setPin(4);
    led.setPin(5);
    log.debug("start listener 0x%lx", &listener);
    listener.start();
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

bool HallApp::Listener::match(EventType type, EventSender *sender)
{
    log.debug("checking match");
    return type == PRESS_EVENT;
}

void HallApp::Listener::handle(Event *event)
{
    log.debug("handle event 0x%lx %d", event, event->getType());

    if (event->getSender() == &(parent->button))
    {
        parent->led.toggle();
        parent->log.debug("got button event");
    }
}