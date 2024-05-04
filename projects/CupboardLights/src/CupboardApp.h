#pragma once
#ifndef _CUPBOARD_H_
#define _CUPBOARD_H_
#include "fclib/App.h"
#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/Hardware.h"
#include "fclib/HomeAssistant.h"
#include "fclib/LedStrip.h"
#include "fclib/Render.h"

using namespace FCLIB;

class CupboardApp : public App
{
public:
    CupboardApp();
    virtual ~CupboardApp();

    void doTask(); // run every loop

protected:
    void onMotionChange(Event *event);
    virtual void setupComplete();

    HW::MultiMotion motion;
    NeoPixelStrip strip1;
    NeoPixelStrip strip2;
    MirrorStrip strips;
    EventListener listener;
    HA::HomeAssistant *ha;
    HA::Device *device;
    HA::LightStrip *haLight;
    HA::MotionSensor *haMotion;
    HA::Number *motion1Pin;
    HA::Number *motion2Pin;
    HA::Number *led1Pin;
    HA::Number *led2Pin;
    HA::Number *led1Count;
    HA::Number *led2Count;
    HA::Select *motionOption;
    HomeAssistantSceneRenderer haRenderer;

    void setMotionType(const char *option);
};

#endif