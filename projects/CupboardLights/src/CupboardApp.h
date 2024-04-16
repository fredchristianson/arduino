#pragma once
#ifndef _CUPBOARD_H_
#define _CUPBOARD_H_
#include "fclib/App.h"
#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/Hardware.h"
#include "fclib/HomeAssistant.h"
#include "fclib/LedStrip.h"

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
};

#endif