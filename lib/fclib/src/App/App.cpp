#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>
#include "fclib/Persistance.h"
#include "fclib/Event.h"

namespace FCLIB
{

    App::App() : log("App")
    {
        wdt_enable(WDTO_4S); // enable watchdog timer
        THE_APP = this;
        this->THE_BOARD = THE_BOARD;
        this->running = false;
    }

    App::~App()
    {
        THE_APP = NULL;
    }

    void App::setupComplete()
    {
        // derived class can override
    }

    void App::loop()
    {
        if (running)
        {
            this->beforeLoop();
            EventManager::processEvents();
            this->runTasks();
            this->afterLoop();
        }
    }

    void App::beforeLoop() {}
    void App::afterLoop() {}
    void App::runTasks()
    {
        TaskQueue::process();
    }

    bool App::setup(Config *config)
    {
        bool startupFail = Persist::get("app", "startupFail", false);
        if (startupFail)
        {
            log.error("Startup failed last time.");
            // set to false so we retry when rebooted
            Persist::set("app", "startupFail", false);
            Persist::store();
            running = false;
            return false;
        }
        // assume startup fails.  will be reset if things configure right and
        // run for 10 seconds
        Persist::set("app", "startupFail", true);

        this->config = config;
        // ensure the TaskQueue is created
        TaskQueue::configure(config);

        bool success = this->beginSetup() &&
                       this->setupLogging() &&
                       this->setupNetwork() &&
                       this->setupDevices() &&
                       this->endSetup();
        if (config->isChanged())
        {
            log.debug("config changed");
            config->save();
        }

        running = success;

        if (success)
        {
            setupComplete();
        }
        // set to startupFail to false if things run ok for 20 seconds
        Task::once([this]()
                   { 
                        this->log.debug("Assuming startup was ok after 10 seconds.");
                        Persist::set("app", "startupFail", false); })
            ->delaySeconds(10);
        return success;
    }

    bool App::beginSetup()
    {

        return true;
    }
    bool App::endSetup()
    {
        return true;
    }
    bool App::setupNetwork()
    {
        return network.setup(config) && network.connect();
    }

    bool App::setupDevices()
    {
        return true;
    }
    bool App::setupLogging()
    {
        log.debug("setupLogging");
        ConfigFile logConfig;
        logConfig.load("/logging.ini");
        configureLogging(this->config->getSection("logging"));
        configureLogging(logConfig.getSection("levels"));
        return true;
    }

    Config *App::getConfig()
    {

        return config;
    }

    App *App::THE_APP;
    Board *App::THE_BOARD;
}