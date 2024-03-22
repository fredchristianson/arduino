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
            getLoop()->run();
        }
    }

    bool App::setup(Config *config)
    {
        this->config = config;
        log.debug("App::setup");
        bool success = getSetup()->setup(config) &&
                       getNetwork()->setup(config) &&
                       getDevices()->setup(config) &&
                       getLoop()->setup(config) &&
                       getEventManager()->setup(config) &&
                       getNetwork()->connect();
        running = success;
        log.debug("App::setup result: %s", success ? "success" : "fail");
        if (config->isChanged())
        {
            log.always("config changed");
            config->save();
        }
        else
        {
            log.always("config did not change");
        }
        if (success)
        {
            setupComplete();
        }
        return success;
    }

    Config *App::getConfig()
    {

        return config;
    }
    AppSetup *App::getSetup()
    {
        if (appSetup == NULL)
        {
            appSetup = createSetup();
        }
        return appSetup;
    }
    AppDevices *App::getDevices()
    {
        if (appDevices == NULL)
        {
            appDevices = createDevices();
        }
        return appDevices;
    }

    AppNetwork *App::getNetwork()
    {
        if (appNetwork == NULL)
        {
            appNetwork = createNetwork();
        }
        return appNetwork;
    }
    AppLoop *App::getLoop()
    {
        if (appLoop == NULL)
        {
            appLoop = createLoop();
        }
        return appLoop;
    }

    AppEventManager *App::getEventManager()
    {
        if (appEventManager == NULL)
        {
            appEventManager = createEventManager();
        }
        return appEventManager;
    }

    AppSetup *App::createSetup()
    {
        AppSetup *setup = new AppSetup();
        setup->app = this;
        return setup;
    }
    AppDevices *App::createDevices()
    {
        AppDevices *devices = new AppDevices();
        devices->app = this;
        return devices;
    }
    AppNetwork *App::createNetwork()
    {
        AppNetwork *network = new AppNetwork();
        network->app = this;
        return network;
    }
    AppLoop *App::createLoop()
    {
        AppLoop *loop = new AppLoop();
        loop->app = this;
        return loop;
    }

    AppEventManager *App::createEventManager()
    {
        AppEventManager *eventManager = new AppEventManager();
        eventManager->app = this;
        return eventManager;
    }
    AppComponent::AppComponent() {}
    AppComponent::~AppComponent() {}
    App *AppComponent::getApp() { return app; }

    Config *AppComponent::getConfig() { return app->getConfig(); }
    AppSetup *AppComponent::getSetup() { return app->getSetup(); }
    AppDevices *AppComponent::getDevices()
    {
        return app->getDevices();
    }
    AppNetwork *AppComponent::getNetwork() { return app->getNetwork(); }
    AppLoop *AppComponent::getLoop() { return app->getLoop(); }
    AppEventManager *AppComponent::getEventManager() { return app->getEventManager(); }
    App *App::THE_APP;
    Board *App::THE_BOARD;
}