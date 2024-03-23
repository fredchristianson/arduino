#ifndef _HALLAPP_H_
#define _HALLAPP_H_
#include "fclib/App.h"
#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/Hardware.h"
#include "fclib/HomeAssistant.h"

using namespace FCLIB;

class HallApp : public App
{
public:
    HallApp();
    virtual ~HallApp();

    void doTask(); // run every loop

protected:
    void onButtonChange(Event *event);
    virtual void setupComplete();
    IntervalTimer halfMinute;
    HW::Button button;
    HW::Led led;

    HA::HomeAssistant *ha;
    HA::Device *haDevice;
    HA::BinarySensor *haButton;
    HA::Led *haLed;
    EventListener listener;
};

#endif