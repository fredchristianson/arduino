#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{
    // todo: setup watchdog timer
    App::App() : log("App")
    {
        wdt_enable(WDTO_4S); // enable watchdog timer
        THE_APP = this;
        this->THE_BOARD = THE_BOARD;
        this->loopTimer = NULL;
        this->useNtp = false;
        this->useMqtt = false;
        this->stopped = false;
        this->loopTimer = new InstantTimer();
    }

    App::~App()
    {
        THE_APP = NULL;
    }

    void App::setSpeed(uint16 framesPerSecond)
    {
        log.always("FramesPerSecond %d", framesPerSecond);
        if (this->loopTimer != NULL)
        {
            this->loopTimer->destroy();
        }
        if (framesPerSecond > 0)
        {
            this->loopTimer = IntervalTimer::createByRate(framesPerSecond);
        }
    }
    void App::loop()
    {
        if (this->stopped)
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

    void App::setLoopTimer(Timer *timer)
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

    bool App::setup(Config *appConfig)
    {
        this->config = appConfig;

        if (this->stopped)
        {
            return false;
        }
        bool success = this->beginSetup() && this->setupLogging() &&
                       this->setupNetwork() && this->connectNetwork() &&
                       this->setupDevices() && this->endSetup();
        if (this->loopTimer == NULL)
        {
            this->loopTimer = new InstantTimer();
        }
        this->stopped = !success;
        return success;
    }
    bool App::setupLogging()
    {
        configureLogging(this->config->getSection("Logging"));
        return true;
    }

    bool App::beginSetup()
    {
        log.debug("begin setup");
        return true;
    }
    bool App::endSetup()
    {
        log.debug("end setup");
        return true;
    }
    bool App::setupNetwork()
    {
        log.debug("networkd setup");
        ConfigSection *section = this->config->getSection("ntp", true);
        ConfigValue *value = section->getValue("connect");
        this->useNtp = (value == NULL || value->toBool());
        return true;
    }
    bool App::setupDevices()
    {
        log.debug("device setup");

        return true;
    }

    bool App::connectNetwork()
    {
        log.info("Connect Network");
        if (this->useNtp)
        {
            int tz_offset = this->config->get("ntp", "tz_offset_minutes", 0);
            log.debug("tz_offset: %d", tz_offset);
            TimeClient::run();
            TimeClient::setTimezoneOffsetMinutes(tz_offset);
        }
        return true;
    }

    bool App::runTests(bool (*runFunc)(), bool stopIfFail)
    {
        bool success = runFunc();
        if (!success && stopIfFail)
        {
            stopped = true;
        }
        return success;
    }

    App *App::THE_APP;
    Board *App::THE_BOARD;
}