#ifndef _HALLAPP_H_
#define _HALLAPP_H_
#include "fclib/App.h"
#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/Component.h"

using namespace FCLIB;
using namespace FCLIB::COMPONENT;

class HallApp : public App, TaskAction
{
public:
    HallApp();
    virtual ~HallApp();

    void doTask() override; // run every loop

protected:
    void onButtonChange(EventSender *sender, EventBooleanData *data);
    virtual void setupComplete();
    IntervalTimer halfMinute;
    Button button;
    Led led;

    struct Listener : public EventListener
    {
        HallApp *parent;
        Logger &log;
        Listener(HallApp *p) : parent(p), log(p->log) {}
        virtual bool match(EventType type, EventSender *sender) override;
        virtual void handle(Event *event) override;
    };
    friend Listener;
    Listener listener;
};

#endif