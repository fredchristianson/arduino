#include <stdlib.h>
#include "../../include/fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{
    App::App() : LOG("App", DEBUG_LEVEL)
    {
        wdt_enable(WDTO_4S); // enable watchdog timer
        THE_APP = this;
        this->THE_BOARD = THE_BOARD;
        this->loopTimer = NULL;
    }

    App::~App()
    {
        THE_APP = NULL;
    }

    void App::setSpeed(uint16 framesPerSecond)
    {
        if (this->loopTimer != NULL)
        {
            this->loopTimer->destroy();
        }
        if (framesPerSecond > 0)
        {
            this->loopTimer = IntervalTimer::createByRate(framesPerSecond);
        }
    }

    void App::beginLoop()
    {
        this->beforeLoop();
    }

    void App::doLoop()
    {
        LOG.conditional(this->loopTimer == NULL, WARN_LEVEL, "Loop timer is null");
        if (this->loopTimer == NULL || this->loopTimer->isComplete())
        {
            this->loopExecute();
        }
    }

    void App::endLoop()
    {
        this->afterLoop();
    }

    void App::setLoopTimer(Timer *timer) { this->loopTimer = timer; }

    App *App::THE_APP;
    Board *App::THE_BOARD;
}