#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{

    // todo: setup watchdog timer
    AppLoop::AppLoop() : AppComponent(), log("AppLoop")
    {
        running = true;
        this->loopTimer = new InstantTimer();
    }

    AppLoop::~AppLoop()
    {
        if (loopTimer != NULL)
        {
            loopTimer->destroy();
        }
    }

    void AppLoop::runOnce()
    {
        if (!this->running)
        {
            log.debug("app is stopped");
        }
        if (this->loopTimer->isComplete())
        {
            if (this->beforeLoop())
            {
                if (this->loopExecute())
                {
                    this->afterLoop();
                }
            }
        }
    }

    void AppLoop::setLoopTimer(Timer *timer)
    {
        if (this->loopTimer)
        {
            this->loopTimer->destroy();
        }
        if (loopTimer == NULL)
        {
            this->loopTimer = new InstantTimer();
        }
        else
        {
            this->loopTimer = timer;
        }
    }

    void AppLoop::setSpeed(uint16 framesPerSecond)
    {
        log.debug("FramesPerSecond %d", framesPerSecond);
        if (this->loopTimer != NULL)
        {
            this->loopTimer->destroy();
        }
        if (framesPerSecond > 0)
        {
            this->loopTimer = IntervalTimer::createByRate(framesPerSecond);
        }
        else
        {
            this->loopTimer = InstantTimer::create();
        }
    }

    bool AppLoop::setup(Config *config)
    {
        long framesPerSecond = config->get("frames_per_second", 0);
        setSpeed(framesPerSecond);
        return true;
    }
}