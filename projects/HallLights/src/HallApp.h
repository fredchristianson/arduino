#ifndef _HALLAPP_H_
#define _HALLAPP_H_
#include "fclib/App.h"
#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/Hardware.h"
#include "fclib/HomeAssistant.h"
#include "fclib/LedStrip.h"

using namespace FCLIB;

class HallApp : public App
{
public:
    HallApp();
    virtual ~HallApp();

    void doTask(); // run every loop

protected:
    void onLedPinChange(int pin);
    void onMotionPinChange(int pin);
    void onLedCountChange(int count);
    void onMotionChange(Event *event);
    void onLightStateChange(Event *event);
    virtual void setupComplete();
    // HW::Led led;

    HA::HomeAssistant *ha;
    HA::Device *haDevice;
    HA::MotionSensor *haMotion;
    HA::LightStrip *haLight;
    HA::Number *haLedCount;
    HA::Number *haLedPin;
    HA::Number *haMotionPin;
    NeoPixelStrip *ledStrip;
    HW::Motion motion;
    HomeAssistantSceneRenderer renderer;
    EventListener listener;
};

#endif